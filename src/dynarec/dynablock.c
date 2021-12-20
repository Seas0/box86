#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <sys/mman.h>
#include <setjmp.h>

#include "debug.h"
#include "box86context.h"
#include "dynarec.h"
#include "emu/x86emu_private.h"
#include "tools/bridge_private.h"
#include "x86run.h"
#include "x86emu.h"
#include "box86stack.h"
#include "callback.h"
#include "emu/x86run_private.h"
#include "x86trace.h"
#include "dynablock.h"
#include "dynablock_private.h"
#include "dynarec_private.h"
#include "elfloader.h"
#ifdef ARM
#include "dynarec_arm.h"
#include "arm_lock_helper.h"
#else
#error Unsupported architecture!
#endif
#include "custommem.h"
#include "khash.h"

KHASH_MAP_INIT_INT(dynablocks, dynablock_t*)

uint32_t X31_hash_code(void* addr, int len)
{
    if(!len) return 0;
    uint8_t* p = (uint8_t*)addr;
	int32_t h = *p;
	for (--len, ++p; len; --len, ++p) h = (h << 5) - h + (int32_t)*p;
	return (uint32_t)h;
}

dynablocklist_t* NewDynablockList(uintptr_t text, int textsz, int direct)
{
    if(!textsz) {
        printf_log(LOG_NONE, "Error, creating a NULL sized Dynablock\n");
        return NULL;
    }
    dynablocklist_t* ret = (dynablocklist_t*)calloc(1, sizeof(dynablocklist_t));
    ret->text = text;
    ret->textsz = textsz;
    if(direct && textsz) {
        ret->direct = (dynablock_t**)calloc(textsz, sizeof(dynablock_t*));
        if(!ret->direct) {printf_log(LOG_NONE, "Warning, fail to create direct block for dynablock @%p\n", (void*)text);}
    }
    dynarec_log(LOG_DEBUG, "New Dynablocklist %p, from %p->%p\n", ret, (void*)text, (void*)(text+textsz));
    return ret;
}

void FreeDynablock(dynablock_t* db, int need_lock)
{
    if(db) {
        if(db->gone)
            return; // already in the process of deletion!
        dynarec_log(LOG_DEBUG, "FreeDynablock(%p), db->block=%p x86=%p:%p father=%p, with %d son(s) already gone=%d\n", db, db->block, db->x86_addr, db->x86_addr+db->x86_size, db->father, db->sons_size, db->gone);
        if(need_lock)
            pthread_mutex_lock(&my_context->mutex_dyndump);
        db->done = 0;
        db->gone = 1;
        // remove from direct if there
        uintptr_t startdb = db->parent->text;
        uintptr_t enddb = db->parent->text + db->parent->textsz;
        if(db->parent->direct) {
            uintptr_t addr = (uintptr_t)db->x86_addr;
            if(addr>=startdb && addr<enddb)
                db->parent->direct[addr-startdb] = NULL;
        }
        // remove jumptable
        setJumpTableDefault(db->x86_addr);
        // remove and free the sons
        for (int i=0; i<db->sons_size; ++i) {
            dynablock_t *son = (dynablock_t*)arm_lock_xchg(&db->sons[i], 0);
            FreeDynablock(son, 0);
        }
        // only the father free the DynarecMap
        if(!db->father) {
            dynarec_log(LOG_DEBUG, " -- FreeDyrecMap(%p, %d)\n", db->block, db->size);
            FreeDynarecMap(db, (uintptr_t)db->block, db->size);
            free(db->sons);
            free(db->instsize);
        }
        free(db);
        if(need_lock)
            pthread_mutex_unlock(&my_context->mutex_dyndump);
    }
}

void FreeDynablockList(dynablocklist_t** dynablocks)
{
    if(!dynablocks)
        return;
    if(!*dynablocks)
        return;
    dynarec_log(LOG_DEBUG, "Free Dynablocklist %p, with Direct Blocks %p\n", *dynablocks, (*dynablocks)->direct);
    if((*dynablocks)->direct) {
        for (int i=0; i<(*dynablocks)->textsz; ++i) {
            if((*dynablocks)->direct[i] && !(*dynablocks)->direct[i]->father) 
                FreeDynablock((*dynablocks)->direct[i], my_context?1:0);
        }
        free((*dynablocks)->direct);
    }
    (*dynablocks)->direct = NULL;

    free(*dynablocks);
    *dynablocks = NULL;
}

void MarkDynablock(dynablock_t* db)
{
    if(db) {
        if(db->father)
            db = db->father;    // mark only father
        if(db->need_test)
            return; // already done
        db->need_test = 1;
        setJumpTableDefault(db->x86_addr);
        for(int i=0; i<db->sons_size; ++i)
            setJumpTableDefault(db->sons[i]->x86_addr);
    }
}

uintptr_t StartDynablockList(dynablocklist_t* db)
{
    if(db)
        return db->text;
    return 0;
}
uintptr_t EndDynablockList(dynablocklist_t* db)
{
    if(db)
        return db->text+db->textsz-1;
    return 0;
}

int IntervalIntersects(uintptr_t start1, uintptr_t end1, uintptr_t start2, uintptr_t end2)
{
    if(start1 > end2 || start2 > end1)
        return 0;
    return 1;
}

void MarkDirectDynablock(dynablocklist_t* dynablocks, uintptr_t addr, uintptr_t size)
{
    if(!dynablocks)
        return;
    if(!dynablocks->direct)
        return;
    uintptr_t startdb = dynablocks->text;
    uintptr_t enddb = startdb + dynablocks->textsz -1;
    uintptr_t start = addr;
    uintptr_t end = addr+size-1;
    if(start<startdb)
        start = startdb;
    if(end>enddb)
        end = enddb;
    dynablock_t *db;
    if(end>startdb && start<enddb)
        for(uintptr_t i = start; i<end; ++i)
            if((db=dynablocks->direct[i-startdb]))
                if(IntervalIntersects((uintptr_t)db->x86_addr, (uintptr_t)db->x86_addr+db->x86_size-1, addr, addr+size+1))
                    MarkDynablock(db);
}

void FreeRangeDynablock(dynablocklist_t* dynablocks, uintptr_t addr, uintptr_t size)
{
    if(!dynablocks)
        return;

    if(dynablocks->direct) {
        dynablock_t* db;
        int ret;
        khint_t k;
        kh_dynablocks_t *blocks = kh_init(dynablocks);
        // copy in a temporary list
        if(dynablocks->direct) {
            uintptr_t startdb = dynablocks->text;
            uintptr_t enddb = startdb + dynablocks->textsz;
            uintptr_t start = addr;
            uintptr_t end = addr+size;
            if(start<startdb)
                start = startdb;
            if(end>enddb)
                end = enddb;
            if(end>startdb && start<enddb)
                for(uintptr_t i = start; i<end; ++i) {
                    db = (dynablock_t*)arm_lock_xchg(&dynablocks->direct[i-startdb], 0);
                    if(db) {
                        if(db->father)
                            db = db->father;
                        if(db->parent==dynablocks) {
                            k = kh_put(dynablocks, blocks, (uintptr_t)db, &ret);
                            kh_value(blocks, k) = db;
                        }
                    }
                }
        }
        // purge the list
        kh_foreach_value(blocks, db,
            FreeDynablock(db, 1);
        );
        kh_destroy(dynablocks, blocks);
    }
}
void MarkRangeDynablock(dynablocklist_t* dynablocks, uintptr_t addr, uintptr_t size)
{
    if(!dynablocks)
        return;
    if(dynablocks->direct) {
        uintptr_t new_addr = addr - dynablocks->maxsz;
        uintptr_t new_size = size + dynablocks->maxsz;
        MarkDirectDynablock(dynablocks, new_addr, new_size);
        // the blocks check before
        for(int idx=(new_addr)>>DYNAMAP_SHIFT; idx<(addr>>DYNAMAP_SHIFT); ++idx)
            MarkDirectDynablock(getDB(idx), new_addr, new_size);
    }
}

dynablock_t* FindDynablockDynablocklist(void* addr, kh_dynablocks_t* dynablocks)
{
    if(!dynablocks)
        return NULL;
    dynablock_t* db;
    kh_foreach_value(dynablocks, db, 
        const uintptr_t s = (uintptr_t)db->block;
        const uintptr_t e = (uintptr_t)db->block+db->size;
        if((uintptr_t)addr>=s && (uintptr_t)addr<e)
            return db->father?db->father:db;
    )
    return NULL;
}

static dynablocklist_t* getDBFromAddress(uintptr_t addr)
{
    const int idx = (addr>>DYNAMAP_SHIFT);
    return getDB(idx);
}

dynablock_t *AddNewDynablock(dynablocklist_t* dynablocks, uintptr_t addr, int* created)
{
    if(!dynablocks) {
        dynarec_log(LOG_INFO, "Warning: Ask to create a dynablock with a NULL dynablocklist (addr=%p)\n", (void*)addr);
        *created = 0;
        return NULL;
    }
    if((addr<dynablocks->text) || (addr>=(dynablocks->text+dynablocks->textsz))) {
        // this should be useless
        //dynarec_log(LOG_INFO, "Warning: Refused to create a Direct Block that is out-of-bound: dynablocks=%p (%p:%p), addr=%p\n", dynablocks, (void*)(dynablocks->text), (void*)(dynablocks->text+dynablocks->textsz), (void*)addr);
        //*created = 0;
        //return NULL;
        return AddNewDynablock(getDBFromAddress(addr), addr, created);
    }
    dynablock_t* block = NULL;
    // first, check if it exist in direct access mode
    if(dynablocks->direct) {
        block = dynablocks->direct[addr-dynablocks->text];
        if(block) {
            dynarec_log(LOG_VERBOSE, "Block already exist in Direct Map\n");
            *created = 0;
            return block;
        }
    }
    
    if (!*created)
        return block;
    
    pthread_mutex_lock(&my_context->mutex_dyndump);
    if(!dynablocks->direct) {
        dynablock_t** p = (dynablock_t**)calloc(dynablocks->textsz, sizeof(dynablock_t*));
        if(arm_lock_storeifnull(&dynablocks->direct, p)!=p)
            free(p);    // someone already create the direct array, too late...
    }

    // create and add new block
    dynarec_log(LOG_VERBOSE, "Ask for DynaRec Block creation @%p\n", (void*)addr);

    block = (dynablock_t*)calloc(1, sizeof(dynablock_t));
    block->parent = dynablocks; 
    dynablock_t* tmp = (dynablock_t*)arm_lock_storeifnull(&dynablocks->direct[addr-dynablocks->text], block);
    pthread_mutex_unlock(&my_context->mutex_dyndump);
    if(tmp !=  block) {
        // a block appeard!
        free(block);
        *created = 0;
        return tmp;
    }
    *created = 1;
    return block;
}

//TODO: move this to dynrec_arm.c and track allocated structure to avoid memory leak
#ifndef ANDROID
static __thread struct __jmp_buf_tag dynarec_jmpbuf;
#else
static __thread jmp_buf dynarec_jmpbuf;
#endif

void cancelFillBlock()
{
    longjmp(&dynarec_jmpbuf, 1);
}

/* 
    return NULL if block is not found / cannot be created. 
    Don't create if create==0
*/
static dynablock_t* internalDBGetBlock(x86emu_t* emu, uintptr_t addr, uintptr_t filladdr, int create, dynablock_t* current, int need_lock)
{
    // try the quickest way first: get parent of current and check if ok!
    dynablocklist_t *dynablocks = NULL;
    dynablock_t* block = NULL;
    if(current && current->done) {
        dynablocks = current->parent;
        if(dynablocks && !(addr>=dynablocks->text && addr<(dynablocks->text+dynablocks->textsz)))
            dynablocks = NULL;
    }
    // nope, lets do the long way
    if(!dynablocks) {
        dynablocks = getDBFromAddress(addr);
        if(!dynablocks) {
            dynablocks = GetDynablocksFromAddress(emu->context, addr);
            if(!dynablocks)
                return NULL;
        }
    }
    // check direct first, without lock
    if(dynablocks->direct/* && (addr>=dynablocks->text) && (addr<(dynablocks->text+dynablocks->textsz))*/)
        if((block = dynablocks->direct[addr-dynablocks->text]))
            return block;

    int created = create;
    block = AddNewDynablock(dynablocks, addr, &created);
    if(!created)
        return block;   // existing block...

    // fill the block
    block->x86_addr = (void*)addr;
    if(need_lock)
        pthread_mutex_lock(&my_context->mutex_dyndump);
    if(sigsetjmp(&dynarec_jmpbuf, 1)) {
        printf_log(LOG_INFO, "FillBlock at %p triggered a segfault, cancelling\n", (void*)addr);
        if(need_lock)
            pthread_mutex_unlock(&my_context->mutex_dyndump);
        return NULL;
    }
    void* ret = FillBlock(block, filladdr);
    if(need_lock)
        pthread_mutex_unlock(&my_context->mutex_dyndump);
    if(!ret) {
        dynarec_log(LOG_DEBUG, "Fillblock of block %p for %p returned an error\n", block, (void*)addr);
        void* old = (void*)arm_lock_storeifref(&dynablocks->direct[addr-dynablocks->text], 0, block);
        if(old!=block && old) { // put it back in place, strange things are happening here!
            dynarec_log(LOG_INFO, "Warning, a wild block appeared at %p: %p\n", (void*)addr, old);
            // Doing nothing else, the block has not been written
        }
        free(block);
        block = NULL;
    }
    // check size
    if(block && block->x86_size) {
        int blocksz = block->x86_size;
        if(dynablocks->maxsz<blocksz) {
            dynablocks->maxsz = blocksz;
            for(int idx=(addr>>DYNAMAP_SHIFT)+1; idx<=((addr+blocksz)>>DYNAMAP_SHIFT); ++idx) {
                dynablocklist_t* dblist;
                if((dblist = getDB(idx)))
                    if(dblist->maxsz<blocksz)
                        dblist->maxsz = blocksz;
            }
        }
        //protectDB((uintptr_t)block->x86_addr, block->x86_size);   // already protected in FillBlock
        // but check if it already need test
        if(block->need_test || (block->father && block->father->need_test))
            return NULL;
        // fill-in jumptable
        addJumpTableIfDefault(block->x86_addr, block->block);
        for(int i=0; i<block->sons_size; ++i)
            addJumpTableIfDefault(block->sons[i]->x86_addr, block->sons[i]->block);
    }

    dynarec_log(LOG_DEBUG, " --- DynaRec Block %s @%p:%p (%p, 0x%x bytes, with %d son(s))\n", created?"created":"recycled", (void*)addr, (void*)(addr+((block)?block->x86_size:0)), (block)?block->block:0, (block)?block->size:0, (block)?block->sons_size:0);

    return block;
}

dynablock_t* DBGetBlock(x86emu_t* emu, uintptr_t addr, int create, dynablock_t** current)
{
    dynablock_t *db = internalDBGetBlock(emu, addr, addr, create, *current, 1);
    if(db && db->done && db->block && (db->need_test || (db->father && db->father->need_test))) {
        if(pthread_mutex_trylock(&my_context->mutex_dyndump))
            return NULL;
        dynablock_t *father = db->father?db->father:db;
        uint32_t hash = (getProtection((uintptr_t)father->x86_addr)&PROT_READ)?X31_hash_code(father->x86_addr, father->x86_size):0;
        if(hash!=father->hash) {
            dynarec_log(LOG_DEBUG, "Invalidating block %p from %p:%p (hash:%X/%X) with %d son(s) for %p\n", father, father->x86_addr, father->x86_addr+father->x86_size, hash, father->hash, father->sons_size, (void*)addr);
            // no more current if it gets invalidated too
            if(*current && father->x86_addr>=(*current)->x86_addr && (father->x86_addr+father->x86_size)<(*current)->x86_addr)
                *current = NULL;
            // Free father, it's now invalid!
            FreeDynablock(father, 0);
            // start again... (will create a new block)
            db = internalDBGetBlock(emu, addr, addr, create, *current, 0);
        } else {
            father->need_test = 0;
            protectDB((uintptr_t)father->x86_addr, father->x86_size);
            // fill back jumptable
            addJumpTableIfDefault(father->x86_addr, father->block);
            for(int i=0; i<father->sons_size; ++i)
                addJumpTableIfDefault(father->sons[i]->x86_addr, father->sons[i]->block);
        }
        pthread_mutex_unlock(&my_context->mutex_dyndump);
    } 
    return db;
}

dynablock_t* DBAlternateBlock(x86emu_t* emu, uintptr_t addr, uintptr_t filladdr)
{
    dynarec_log(LOG_DEBUG, "Creating AlternateBlock at %p for %p\n", (void*)addr, (void*)filladdr);
    int create = 1;
    dynablock_t *db = internalDBGetBlock(emu, addr, filladdr, create, NULL, 1);
    if(db && db->done && db->block && (db->need_test || (db->father && db->father->need_test))) {
        if(pthread_mutex_trylock(&my_context->mutex_dyndump))
            return NULL;
        dynablock_t *father = db->father?db->father:db;
        uint32_t hash = (getProtection((uintptr_t)father->x86_addr)&PROT_READ)?X31_hash_code(father->x86_addr, father->x86_size):0;
        if(hash!=father->hash) {
            dynarec_log(LOG_DEBUG, "Invalidating alt block %p from %p:%p (hash:%X/%X) with %d son(s) for %p\n", father, father->x86_addr, father->x86_addr+father->x86_size, hash, father->hash, father->sons_size, (void*)addr);
            // Free father, it's now invalid!
            FreeDynablock(father, 0);
            // start again... (will create a new block)
            db = internalDBGetBlock(emu, addr, filladdr, create, NULL, 0);
        } else {
            father->need_test = 0;
            protectDB((uintptr_t)father->x86_addr, father->x86_size);
            // fill back jumptable
            addJumpTableIfDefault(father->x86_addr, father->block);
            for(int i=0; i<father->sons_size; ++i)
                addJumpTableIfDefault(father->sons[i]->x86_addr, father->sons[i]->block);
        }
        pthread_mutex_unlock(&my_context->mutex_dyndump);
    } 
    return db;
}

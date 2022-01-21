#ifndef __LIBRARY_PRIVATE_H_
#define __LIBRARY_PRIVATE_H_
#include <stdint.h>

#include "custommem.h"
#include "khash.h"
#include "wrappedlibs.h"
#include "box86context.h"

typedef struct lib_s    lib_t;
typedef struct bridge_s bridge_t;
typedef struct kh_bridgemap_s kh_bridgemap_t;
typedef struct kh_mapsymbols_s kh_mapsymbols_t;

typedef struct x86emu_s x86emu_t;
typedef void (*wrapper_t)(x86emu_t* emu, uintptr_t fnc);

typedef struct symbol2_s {
    wrapper_t    w;
    const char*  name;
    int          weak;
} symbol2_t;

KHASH_MAP_DECLARE_STR(symbolmap, wrapper_t)
KHASH_MAP_DECLARE_STR(symbol2map, symbol2_t)
KHASH_MAP_DECLARE_STR(datamap, uint32_t)


#ifndef MAX_PATH
#define MAX_PATH 4096
#endif

typedef struct wlib_s {
    bridge_t        *bridge;
    void*           lib;        // dlopen result
    void*           priv;       // actual private
    void*           p2;         // second private
    void*           box86lib;   // ref to the dlopen on box86 itself from context
    char*           altprefix;  // if function names are mangled..
    int             needed;
    char**          neededlibs;
} wlib_t;

typedef struct nlib_s {
    int             elf_index;
    int             finalized;
    kh_mapsymbols_t *mapsymbols;
    kh_mapsymbols_t *weaksymbols;
    kh_mapsymbols_t *localsymbols;
} nlib_t;

typedef struct library_s {
    char*               name;   // <> path
    char*               path;   // original path
    int                 nbdot;  // nombre of "." after .so
    int                 type;   // 0: native(wrapped) 1: emulated(elf) -1: undetermined
    int                 active;
    wrappedlib_fini_t   fini;
    wrappedlib_get_t    get;        // get weak and no weak
    wrappedlib_get_t    getnoweak;  // get only non weak symbol
    wrappedlib_get_t    getlocal;
    union {
        wlib_t  w;
        nlib_t  n;
    }                   priv;  // private lib data
    box86context_t      *context;   // parent context
    kh_bridgemap_t      *bridgemap;
    kh_symbolmap_t      *symbolmap;
    kh_symbolmap_t      *wsymbolmap;
    kh_symbolmap_t      *mysymbolmap;
    kh_symbolmap_t      *wmysymbolmap;
    kh_symbolmap_t      *stsymbolmap;
    kh_symbolmap_t      *wstsymbolmap;
    kh_symbol2map_t     *symbol2map;
    kh_datamap_t        *datamap;
    kh_datamap_t        *wdatamap;
    kh_datamap_t        *mydatamap;
    char                *altmy;      // to avoid duplicate symbol, like with SDL1/SDL2
    needed_libs_t       needed;
    needed_libs_t       dependedby;
    lib_t               *maplib;    // local maplib, for dlopen'd library with LOCAL binding (most of the dlopen)
} library_t;
void add_neededlib(needed_libs_t* needed, library_t* lib);
void free_neededlib(needed_libs_t* needed);
void add_dependedbylib(needed_libs_t* dependedby, library_t* lib);
void free_dependedbylib(needed_libs_t* dependedby);

// type for map elements
typedef struct map_onesymbol_s {
    const char* name;
    wrapper_t   w;
    int         weak;
} map_onesymbol_t;
typedef struct map_onesymbol2_s {
    const char* name;
    wrapper_t   w;
    int         weak;
    const char* name2;
} map_onesymbol2_t;
typedef struct map_onedata_s {
    const char* name;
    uint32_t    sz;
    int         weak;
} map_onedata_t;

int getSymbolInMaps(library_t*lib, const char* name, int noweak, uintptr_t *addr, uint32_t *size, int version, const char* vername, int local);  // Add bridges to functions

#endif //__LIBRARY_PRIVATE_H_

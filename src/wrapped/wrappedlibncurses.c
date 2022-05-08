#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include "wrappedlibs.h"

#include "debug.h"
#include "wrapper.h"
#include "bridge.h"
#include "librarian/library_private.h"
#include "x86emu.h"
#include "emu/x86emu_private.h"
#include "callback.h"
#include "librarian.h"
#include "box86context.h"
#include "emu/x86emu_private.h"
#include "myalign.h"
#include "globalsymbols.h"

const char* libncursesName = "libncurses.so.5";
#define LIBNAME libncurses

typedef int32_t (*iFppp_t)(void*, void*, void*);

#define ADDED_FUNCTIONS() GO(stdscr, void*)
#include "generated/wrappedlibncursestypes.h"

#include "wrappercallback.h"

EXPORT int my_mvwprintw(x86emu_t* emu, void* win, int y, int x, void* fmt, void* b)
{
    char* buf = NULL;
    #ifndef NOALIGN
    myStackAlign((const char*)fmt, b, emu->scratch);
    PREPARE_VALIST;
    iFppp_t f = (iFppp_t)vasprintf;
    f(&buf, fmt, VARARGS);
    #else
    vasprintf(&buf, fmt, b);
    #endif
    // pre-bake the fmt/vaarg, because there is no "va_list" version of this function
    int ret = my->mvwprintw(win, y, x, buf);
    free(buf);
    return ret;
}

EXPORT int my_printw(x86emu_t* emu, void* fmt, void* b)
{
    #ifndef NOALIGN
    myStackAlign((const char*)fmt, b, emu->scratch);
    PREPARE_VALIST;
    return my->vw_printw(my->stdscr, fmt, VARARGS);
    #else
    return my->vw_printw(my->stdscr, fmt, b);
    #endif
}

EXPORT int my_wprintw(x86emu_t* emu, void* win, void* fmt, void* b)
{
    #ifndef NOALIGN
    myStackAlign((const char*)fmt, b, emu->scratch);
    PREPARE_VALIST;
    return my->vw_printw(win, fmt, VARARGS);
    #else
    return my->vw_printw(win, fmt, b);
    #endif
}

EXPORT int my_mvprintw(x86emu_t* emu, int x, int y, void* fmt, void* b)
{
    char* buf = NULL;
    #ifndef NOALIGN
    myStackAlign((const char*)fmt, b, emu->scratch);
    PREPARE_VALIST;
    iFppp_t f = (iFppp_t)vasprintf;
    f(&buf, fmt, VARARGS);
    #else
    vasprintf(&buf, fmt, b);
    #endif
    // pre-bake the fmt/vaarg, because there is no "va_list" version of this function
    int ret = my->mvprintw(x, y, buf);
    free(buf);
    return ret;
}

EXPORT int my_vw_printw(x86emu_t *emu, void* win, void* fmt, void* b) {
    #ifndef NOALIGN
    myStackAlign((const char*)fmt, b, emu->scratch);
    PREPARE_VALIST;
    return my->vw_printw(win, fmt, VARARGS);
    #else
    // other platform don't need that
    return my->vw_printw(win, fmt, b);
    #endif
}
EXPORT int my_vwprintw(x86emu_t *emu, void* win, void* fmt, void* b) __attribute__((alias("my_vw_printw")));

EXPORT void* my_initscr()
{
    void* ret = my->initscr();
    my_checkGlobalTInfo();
    return ret;
}

#define CUSTOM_INIT \
    getMy(lib); \
    setNeededLibs(&lib->priv.w, 1, "libtinfo.so.5");

#define CUSTOM_FINI \
    freeMy();

#include "wrappedlib_init.h"

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdint.h>

typedef struct config_s
{
    char *program_name;

    int no_banner;
    int log_level;
    int dump;
#ifdef DYNAREC
    int dynarec_log_level;
    int dynarec_dump;
    int dynarec_enabled;
    int dynarec_forced_all_addr;
    uintptr_t no_dynarec_start;
    uintptr_t no_dynarec_end;

    int dynarec_bigblock;
    int dynarec_strongmem;
#endif

#ifdef HAVE_TRACE
    int trace_xmm;
    int trace_emm;
    int trace_start;
#ifdef DYNAREC
    int trace_dynarec;
#endif
#endif
    int dlsym_error;

    int x11_color16;
    int x11_threads;
    int x11_glx;

    char *libgl;

    int allow_missing_libs;

    int no_pulse;
    int no_gtk;
    int no_vulkan;

    int fix_64bit_inodes;

    int jit_gdb;

    char *lib_path;
    int lib_path_merge;
    char *path;
    int path_merge;

    char *emulated_libs;

    int no_sigsegv;
    int no_sigill;

    int trace;
    int trace_init;
    char *trace_file_path;

} config_t;

typedef struct library_config_s
{
    char *library_name;
    char *value;
} library_config_t;

// parse a config file
config_t *config_parse(FILE *file);

// parse a section of a config file
int config_parse_section(config_t *config, FILE *file);

// only a dummy file now, wait for implementation

#endif
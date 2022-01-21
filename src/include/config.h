#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdio.h>
#include <stdint.h>

typedef enum
{
    Undefined = -1,
    False = 0,
    True = 1
} tristate_t;
typedef struct config_s
{
    char *program_name;

    tristate_t no_banner;
    tristate_t log_level;
    tristate_t dump;
#ifdef DYNAREC
    tristate_t dynarec_log_level;
    tristate_t dynarec_dump;
    tristate_t dynarec_enabled;
    tristate_t dynarec_forced_all_addr;
    uintptr_t no_dynarec_start;
    uintptr_t no_dynarec_end;

    tristate_t dynarec_bigblock;
    tristate_t dynarec_strongmem;
#endif

#ifdef HAVE_TRACE
    tristate_t trace_xmm;
    tristate_t trace_emm;
    unsigned long long trace_start;
#ifdef DYNAREC
    tristate_t trace_dynarec;
#endif
#endif
    tristate_t dlsym_error;

    tristate_t x11_color16;
    tristate_t x11_threads;
    tristate_t x11_glx;

    char *libgl;

    tristate_t allow_missing_libs;

    tristate_t no_pulse;
    tristate_t no_gtk;
    tristate_t no_vulkan;

    tristate_t fix_64bit_inodes;

    tristate_t jit_gdb;

    path_collection_t *path;
    tristate_t path_merge;
    path_collection_t *lib_path;
    tristate_t lib_path_merge;
    path_collection_t *lib_preload;
    tristate_t lib_preload_merge;

    path_collection_t *emulated_libs;

    tristate_t no_sigsegv;
    tristate_t no_sigill;

    tristate_t trace;
    tristate_t trace_init;
    char *trace_file_path;

} config_t;
const config_t config_default = {
    .program_name = NULL,
    .no_banner = Undefined,
    .log_level = Undefined,
    .dump = Undefined,
#ifdef DYNAREC
    .dynarec_log_level = Undefined,
    .dynarec_dump = Undefined,
    .dynarec_enabled = Undefined,
    .dynarec_forced_all_addr = Undefined,
    .no_dynarec_start = Undefined,
    .no_dynarec_end = Undefined,

    .dynarec_bigblock = Undefined,
    .dynarec_strongmem = Undefined,
#endif

#ifdef HAVE_TRACE
    .trace_xmm = Undefined,
    .trace_emm = Undefined,
    .trace_start = Undefined,
#ifdef DYNAREC
    .trace_dynarec = Undefined,
#endif
#endif
    .dlsym_error = Undefined,
    .x11_color16 = Undefined,
    .x11_threads = Undefined,
    .x11_glx = Undefined,

    .libgl = NULL,

    .allow_missing_libs = Undefined,
    .no_pulse = Undefined,
    .no_gtk = Undefined,
    .no_vulkan = Undefined,
    .fix_64bit_inodes = Undefined,
    .jit_gdb = Undefined,

    .path = NULL,
    .path_merge = Undefined,
    .lib_path = NULL,
    .lib_path_merge = Undefined,
    .lib_preload = NULL,
    .lib_preload_merge = Undefined,

    .emulated_libs = NULL,

    .no_sigsegv = Undefined,
    .no_sigill = Undefined,

    .trace = Undefined,
    .trace_init = Undefined,
    .trace_file_path = NULL};

extern config_t *config_table;

// Parse a config file
int ParseConfigFile(FILE *file, config_t **config, size_t *n);

// Parse a section of a config file
int ParseConfigSection(FILE *file, config_t *config);

// Parse a line of a config file
int ParseConfigLine(char *line, config_t *config);

// Initialize config
int InitConfigFile();

// Free config
void FreeConfig();

// Get Option
char *GetOption(const char *name);

#endif
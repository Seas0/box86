#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <errno.h>
#include <unistd.h>

#include "box86context.h"
#include "fileutils.h"
#include "pathcoll.h"
#include "config.h"

typedef struct config_s config_t;
config_t *config_table;
size_t config_table_size;

const char *local_config_path = "box86.conf";

const char *system_config_path = "/etc/box86/box86.conf";

inline static int8_t isSpace(char c)
{
    return c == ' ' || c == '\r' || c == '\x09';
}
inline static int8_t isValidCharacter(char c)
{
    return ('0' <= c && c <= '9') ||
           ('A' <= c && c <= 'Z') ||
           ('a' <= c && c <= 'z') ||
           c == '_' || c == '.';
}
inline static int8_t boolEval(char *c)
{
    if (!strcasecmp(c, "true") || !strcasecmp(c, "yes") || !strcasecmp(c, "on") || !strcasecmp(c, "1"))
        return 1;
    if (!strcasecmp(c, "false") || !strcasecmp(c, "no") || !strcasecmp(c, "off") || !strcasecmp(c, "0"))
        return 0;
    return -1;
}

/*
    Parse a ini file
*/
int ParseConfigFile(FILE *file, config_t **config, size_t *n)
{
}

/*
    Find and read config
*/
int InitConfigFile()
{
    char *user_config_path = getenv("HOME");
    strcat(user_config_path, "/.box86/box86.conf");

    const char *config_paths[] = {local_config_path,
                                  user_config_path,
                                  system_config_path};

    for (int i = 0; i < 3; i++)
    {
        config_t *config;
        size_t size;
        FILE *file = fopen(config_paths[i], "r");
        if (!file)
            continue;
        if (ParseConfigFile(file, &config, &size))
        {
            fclose(file);
            continue;
        }
        fclose(file);
        config_table = realloc(config_table, config_table_size + size * sizeof(config_t));
        memcpy(config_table + config_table_size * sizeof(config_t), config, size * sizeof(config_t));
        config_table_size += size;
    }

    return 0;
}
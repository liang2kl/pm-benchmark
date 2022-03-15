#ifndef CONFIG_H
#define CONFIG_H

#include <sys/types.h>
#include <stdio.h>

#define MAX_TASKS 128

struct task_config {
    enum {
        READ = 1,
        WRITE = 2
    } type;

    enum {
        SEQUENTIAL = 0,
        RANDOM = 1
    } pattern;
    
    size_t block_size;
    size_t mapped_len;
    char dev[63];
};

struct task_config **parse_config(const char *file_name, int *num);

void print_config(FILE *stream, struct task_config *config);
const char *pattern_name(struct task_config *config);
const char *type_name(struct task_config *config);
#endif
#ifndef CONFIG_H
#define CONFIG_H

#include <sys/types.h>

#define MAX_TASKS 64

struct task_config {
    enum {
        LATENCY = 0,
        READ_BW = 1,
        WRITE_BW = 2
    } type;

    enum {
        SEQUENTIAL = 0,
        RANDOM = 1
    } pattern;
    
    size_t block_size;
    char dev[63];
};

struct task_config **parse_config(const char *file_name, int *num);

void print_config(struct task_config *config);

#endif
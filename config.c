#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct task_config *new_config() {
    struct task_config *conf = malloc(sizeof(struct task_config));
    conf->block_size = 256;
    strcpy(conf->dev, "");
    conf->pattern = SEQUENTIAL;
    conf->type = LATENCY;
    return conf;
}

struct task_config **parse_config(const char *file_name, int *num) {
    FILE *file = fopen(file_name, "r");
    if (!file) {
        perror("Fail to open config file");
        return NULL;
    }
    struct task_config **configs = malloc(sizeof(struct task_config*) * MAX_TASKS);
    configs[0] = new_config();
    struct task_config *conf = configs[0];
    
    char line[256];
    char *pos, *pos_val, *saveptr;
    int config_num = 0;

    for (int line_num = 1; fgets(line, sizeof(line), file); line_num++) {
        if (strcmp(line, "TASK\n") == 0) {
            if (config_num == MAX_TASKS) {
                fprintf(stderr, "[Warning] Tasks exceed max num %d\n", MAX_TASKS);
                goto done;
            }
            configs[config_num] = new_config();
            conf = configs[config_num];
            config_num++;
            continue;
        }
        pos = strtok(line, " \t\n");
        if (!pos) {
            continue;
        }

        pos = strtok_r(pos, "=", &saveptr);
        pos_val = strtok_r(NULL, "=", &saveptr);

        if (!pos || !pos_val) {
            goto line_invalid;
        }

        if (strcmp(pos, "dev") == 0) {
            strcpy(conf->dev, pos_val);
        } else if (strcmp(pos, "type") == 0) {
            if (strcmp(pos_val, "latency") == 0) {
                conf->type = LATENCY;
            } else if (strcmp(pos_val, "read_bw") == 0) {
                conf->type = READ_BW;
            } else if (strcmp(pos_val, "write_bw") == 0) {
                conf->type = WRITE_BW;
            } else {
                goto line_invalid;
            }
        } else if (strcmp(pos, "pattern") == 0) {
            if (strcmp(pos_val, "sequential") == 0) {
                conf->pattern = SEQUENTIAL;
            } else if (strcmp(pos_val, "random") == 0) {
                conf->type = RANDOM;
            } else {
                goto line_invalid;
            }
        } else if (strcmp(pos, "block_size") == 0) {
            conf->block_size = atoi(pos_val);
        } else {
            goto line_invalid;
        }
        
        continue;
line_invalid:
    fprintf(stderr, "[Warning] Invalid config at line #%d\n", line_num);
    }

done:
    *num = config_num;
    fclose(file);
    return configs;
}

void print_config(struct task_config *config) {
    puts("--- Config ---");
    printf("Device: %s\n", config->dev);
    printf("Type: %d\n", config->type);
    printf("Pattern: %d\n", config->pattern);
    printf("Block size: %lu\n", config->block_size);
    puts("--------------");
}
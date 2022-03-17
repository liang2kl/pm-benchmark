#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct task_config *new_config() {
    struct task_config *conf = malloc(sizeof(struct task_config));
    conf->block_size = 256;
    strcpy(conf->dev, "");
    conf->pattern = SEQUENTIAL;
    conf->type = READ;
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
        if (strncmp(line, "TASK", 4) == 0) {
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
            if (strcmp(pos_val, "read") == 0) {
                conf->type = READ;
            } else if (strcmp(pos_val, "write") == 0) {
                conf->type = WRITE;
            } else {
                goto line_invalid;
            }
        } else if (strcmp(pos, "pattern") == 0) {
            if (strcmp(pos_val, "sequential") == 0) {
                conf->pattern = SEQUENTIAL;
            } else if (strcmp(pos_val, "random") == 0) {
                conf->pattern = RANDOM;
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

const char *type_name(struct task_config *config) {
    switch (config->type) {
        case WRITE:
            return "write";
        case READ:
            return "read";
    }
}

const char *pattern_name(struct task_config *config) {
    switch (config->pattern) {
        case SEQUENTIAL:
            return "sequential";
        case RANDOM:
            return "random";
    }
}

void print_config(FILE *stream, struct task_config *config) {
    fprintf(stream, "( ");
    fprintf(stream, "Device: %s ", config->dev);
    fprintf(stream, "Type: %s ", type_name(config));
    fprintf(stream, "Pattern: %s ", pattern_name(config));
    fprintf(stream, "Block size: %lu ", config->block_size);
    fprintf(stream, ")\n");
}

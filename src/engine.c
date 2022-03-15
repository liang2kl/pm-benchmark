#include "engine.h"
#include "config.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WR_SIZE 12800000000
#define MAX_WR_OP 100000
#define MAX_RD_SIZE 25600000000
#define MAX_RD_OP 1000000

void test_write(char *pmem_addr, struct task_config *conf, struct task_result *res) {
    clock_t time;
    int iter = MIN(MAX_WR_OP, MAX_WR_SIZE / conf->block_size);
    char *buf = malloc(conf->block_size);
    size_t offset = 0;

    MEASURE_TIME(
        time, for (int i = 0; i < iter; i++) {
            offset += conf->block_size;
            if (offset > conf->mapped_len) {
                offset = 0;
            }
            pmem_memmove_persist(pmem_addr + offset, buf, conf->block_size);
        })

    free(buf);

    res->bandwidth = iter * conf->block_size * CLOCKS_PER_SEC / (double)time;
    res->latency = (double)time * 1000000000 / CLOCKS_PER_SEC / iter;
}

void test_read(char *pmem_addr, struct task_config *conf, struct task_result *res) {
    clock_t time;
    int iter = MIN(MAX_RD_OP, MAX_RD_SIZE / conf->block_size);
    char *buf = malloc(conf->block_size);
    size_t offset = 0;

    MEASURE_TIME(
        time, for (int i = 0; i < iter; i++) {
            offset += conf->block_size;
            if (offset > conf->mapped_len) {
                offset = 0;
            }
            memcpy(buf, pmem_addr + offset, conf->block_size);
        })

    free(buf);

    res->bandwidth = iter * conf->block_size * CLOCKS_PER_SEC / (double)time;
    res->latency = (double)time * 1000000000 / CLOCKS_PER_SEC / iter;
}
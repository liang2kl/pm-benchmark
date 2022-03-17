#include "engine.h"
#include "config.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#define IDEAL_WR_SIZE 2560000000  // approx 2GB
#define MAX_WR_OP 10000000
#define IDEAL_RD_SIZE 5120000000  // approx 5GB
#define MAX_RD_OP 700000

#define INTERVAL_LEN 268435456UL  // 0.25GB
#define INTERVAL_NUM 448
#define TEST_LEN 120259084288UL   // 112GB
#define SUB_INTERVAL_LEN 4194304  // 4MB

#define GET_OFFSET(offset)                                             \
    {                                                                  \
        size_t __next = (offset + INTERVAL_LEN) % TEST_LEN;            \
        offset = __next < offset ? __next + SUB_INTERVAL_LEN : __next; \
    }

void test_write(char *pmem_addr, struct task_config *conf, struct task_result *res) {
    clock_t time;
    int oper_num = MIN(MAX_WR_OP, IDEAL_WR_SIZE / conf->block_size);
    char *buf = malloc(conf->block_size);
    size_t offset = 0;
    size_t max_offset = conf->mapped_len * 2 / 3;

    MEASURE_TIME(
        time, for (int i = 0; i < oper_num; i++) {
            if (conf->pattern == RANDOM) {
                GET_OFFSET(offset);
            } else {
                offset += conf->block_size;
                if (offset > TEST_LEN) {
                    offset = 0;
                }
            }
            char * addr = pmem_addr + offset;

            memcpy(addr, buf, conf->block_size);

            asm volatile(
                "mfence\n"
                "clwb %0 \n"
                : "+m"(addr));
        })

    free(buf);

    res->bandwidth = oper_num * conf->block_size * CLOCKS_PER_SEC / (double)time;
    res->latency = (double)time * 1000000000 / CLOCKS_PER_SEC / oper_num;
}

void test_read(char *pmem_addr, struct task_config *conf, struct task_result *res) {
    clock_t time;
    int oper_num = MIN(MAX_RD_OP, IDEAL_RD_SIZE / conf->block_size);
    char *buf = malloc(conf->block_size);
    size_t offset = 0;
    size_t max_offset = conf->mapped_len * 2 / 3;

    MEASURE_TIME(
        time, for (int i = 0; i < oper_num; i++) {
            if (conf->pattern == RANDOM) {
                GET_OFFSET(offset);
            } else {
                offset += conf->block_size;
                if (offset > TEST_LEN) {
                    offset = 0;
                }
            }

            memcpy(buf, pmem_addr + offset, conf->block_size);
        })

    free(buf);

    res->bandwidth = oper_num * conf->block_size * CLOCKS_PER_SEC / (double)time;
    res->latency = (double)time * 1000000000 / CLOCKS_PER_SEC / oper_num;
}
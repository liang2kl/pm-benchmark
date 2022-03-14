#include "bandwidth.h"
#include "config.h"
#include "utils.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 5120000000
#define MAX_ITERATION 1000000

static int rand_addr(void *pmem_addr, int i) {

}

long test_write_bandwidth(char *pmem_addr, struct task_config *conf) {
    clock_t time;
    int i;

    // TODO: Add constraint with device capacity
    MEASURE_TIME_FOR(time, for (i = 0; i < MAX_ITERATION && i * conf->block_size < MAX_SIZE; i++),
                     pmem_persist(pmem_addr + i * PM_ACCESS_GRANULARITY, conf->block_size))

    return MIN(MAX_ITERATION, i) * conf->block_size * CLOCKS_PER_SEC / time;
}

long test_read_bandwidth(char *pmem_addr, struct task_config *conf) {
    clock_t time;
    int i;
    char *buf = malloc(conf->block_size);

    // TODO: Add constraint with device capacity
    MEASURE_TIME_FOR(time, for (i = 0; i < MAX_ITERATION && i * conf->block_size < MAX_SIZE; i++),
                     memcpy(buf, pmem_addr + i * PM_ACCESS_GRANULARITY, conf->block_size))

    free(buf);
    return MIN(MAX_ITERATION, i) * conf->block_size * CLOCKS_PER_SEC / time;
}
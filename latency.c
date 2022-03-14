#include "latency.h"
#include "utils.h"

#define LAT_ATTEMPTS 1000000

// FIXME: Penalty of calling clock()
double test_latency(char *pmem_addr, struct task_config *conf) {
    clock_t time;
    char val;

    MEASURE_TIME(time, for (u_int64_t i = 0; i < LAT_ATTEMPTS; i++) {
        val = *(pmem_addr + i * PM_ACCESS_GRANULARITY * 256);
    });

    // MEASURE_TIME_FOR(time, for (u_int64_t i = 0; i < LAT_ATTEMPTS; i++),
    //                  val = *(pmem_addr + i * PM_ACCESS_GRANULARITY * 256))

    return (double)time * 1000000000 / LAT_ATTEMPTS / CLOCKS_PER_SEC;
}

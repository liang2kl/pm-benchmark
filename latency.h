#ifndef LATENCY_H
#define LATENCY_H

#include <libpmem.h>
#include "config.h"

#define PM_ACCESS_GRANULARITY 256

double test_latency(char *pmem_addr, struct task_config *conf);

#endif
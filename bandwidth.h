#ifndef BANDWIDTH_H
#define BANDWIDTH_H

#include <libpmem.h>
#include "config.h"

long test_write_bandwidth(char *pmem_addr, struct task_config *conf);
long test_read_bandwidth(char *pmem_addr, struct task_config *conf);

#endif
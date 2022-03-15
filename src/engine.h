#ifndef ENGINE_H
#define ENGINE_H

#include <libpmem.h>
#include "config.h"
#include "result.h"
#include "utils.h"

void test_write(char *pmem_addr, struct task_config *conf, struct task_result *res);
void test_read(char *pmem_addr, struct task_config *conf, struct task_result *res);

#endif
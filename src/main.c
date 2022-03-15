#include <err.h>
#include <errno.h>
#include <libpmem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "engine.h"
#include "config.h"
#include "utils.h"

// For Device DAX mappings, len must be equal to either 0 or the exact size of the device.
#define PMEM_LEN 0


int main(int argc, char **argv) {
    char *pmem_addr;
    size_t mapped_len;
    int is_pmem;
    int task_num;
    struct task_config **configs;
    struct task_result *results;

    if (argc < 2) {
        puts("Invalid arguments\n");
        puts("Usage: <program> <config_path>\n");
        exit(1);
    }

    // disables mapping address randomization
    putenv("PMEM_MMAP_HINT=0x10000000000");
    // forces libpmem to always use the non-temporal move instructions
    putenv("PMEM_MOVNT_THRESHOLD=0");
    // never issue any of CLFLUSH, CLFLUSHOPT or CLWB instructions on Intel hardware
    putenv("PMEM_NO_FLUSH=1");

        configs = parse_config(argv[1], &task_num);

    if (!configs) {
        exit(1);
    }

    results = malloc(sizeof(struct task_result) * task_num);

    fprintf(stderr, "Running tasks with configs in %s\n", argv[1]);

    for (int i = 0; i < task_num; i++) {
        fprintf(stderr, "Running task %d/%d ", i + 1, task_num);
        print_config(stderr, configs[i]);

        if ((pmem_addr = pmem_map_file(configs[i]->dev, PMEM_LEN, PMEM_FILE_CREATE,
                                       0666, &mapped_len, &is_pmem)) == NULL) {
            printf("Fail to map address for dev '%s': %s\n", configs[i]->dev, strerror(errno));
            continue;
        }


        configs[i]->mapped_len = mapped_len;

        switch (configs[i]->type) {
            case WRITE:
                test_write(pmem_addr, configs[i], &results[i]);
                break;
            case READ:
                test_read(pmem_addr, configs[i], &results[i]);
                break;
        }

        // Output as csv
        printf("%s,%s,%s,%ld,%f,%f\n", configs[i]->dev, type_name(configs[i]),
               pattern_name(configs[i]), configs[i]->block_size, results[i].latency,
               results[i].bandwidth);

        free(configs[i]);
    }

    free(configs);
    free(results);

    return 0;
}
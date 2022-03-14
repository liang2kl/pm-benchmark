#include <err.h>
#include <errno.h>
#include <libpmem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "bandwidth.h"
#include "config.h"
#include "latency.h"
#include "utils.h"

// For Device DAX mappings, len must be equal to either 0 or the exact size of the device.
#define PMEM_LEN 0


int main(int argc, char **argv) {
    char *pmem_addr;
    size_t mapped_len;
    int is_pmem, attempts;

    if (argc < 2) {
        puts("Invalid arguments\n");
        puts("Usage: <program> <config_path>\n");
        exit(1);
    }

    int task_num;
    struct task_config **configs = parse_config(argv[1], &task_num);

    if (!configs) {
        exit(1);
    }

    for (int i = 0; i < task_num; i++) {
        printf("Running task %d (%s)\n", i, configs[i]->dev);
        // print_config(configs[i]);

        if ((pmem_addr = pmem_map_file(configs[i]->dev, PMEM_LEN, PMEM_FILE_CREATE,
                                       0666, &mapped_len, &is_pmem)) == NULL) {
            printf("Fail to map address for dev '%s': %s\n", configs[i]->dev, strerror(errno));
            continue;
        }

        long bw;
        double lat;
        switch (configs[i]->type) {
            case LATENCY:
                lat = test_latency(pmem_addr, configs[i]);
                printf("Latency: %.2f ns\n", lat);
                break;
            case WRITE_BW:
                bw = test_write_bandwidth(pmem_addr, configs[i]);
                printf("Bandwidth: %ld MB/s\n", bw / 1000000);
                break;
            case READ_BW:
                bw = test_read_bandwidth(pmem_addr, configs[i]);
                printf("Bandwidth: %ld MB/s\n", bw / 1000000);
                break;
        }
        puts("");
        free(configs[i]);
    }

    free(configs);

    return 0;
}
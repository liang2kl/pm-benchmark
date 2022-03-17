#ifndef UTILS_H
#define UTILS_H

#include <time.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define MEASURE_TIME(result, body)       \
    {                                    \
        clock_t __begin_time = clock();  \
        body;                            \
        result = clock() - __begin_time; \
    }

#endif

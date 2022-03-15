#ifndef UTILS_H
#define UTILS_H

#include <time.h>

#define PM_ACCESS_GRANULARITY 256

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define MEASURE_TIME(result, body)       \
    {                                    \
        clock_t __begin_time = clock();  \
        body;                            \
        result = clock() - __begin_time; \
    }

#define MEASURE_TIME_FOR(result, for_body, st) \
    {                                          \
        clock_t __time, __total = 0;           \
        for_body {                             \
            MEASURE_TIME(__time, st);          \
            __total += __time;                 \
        }                                      \
        result = __total;                      \
    }

#endif

#include "time_util.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <time.h>
#endif

uint64_t get_time_ms(void) {
#if defined(_WIN32) || defined(_WIN64)

    static LARGE_INTEGER frequency;
    static int initialized = 0;
    LARGE_INTEGER counter;

    if (!initialized) {
        QueryPerformanceFrequency(&frequency);
        initialized = 1;
    }

    QueryPerformanceCounter(&counter);

    return (uint64_t)(((uint64_t)counter.QuadPart * 1000ULL) /
                      (uint64_t)frequency.QuadPart);

#else

    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return ts.tv_sec * 1000ULL + ts.tv_nsec / 1000000ULL;

#endif
}

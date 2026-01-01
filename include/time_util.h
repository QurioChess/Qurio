#pragma once

#include <stdint.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <time.h>
#endif

// get_time_ms(): returns current time in milliseconds
// Cross-platform: uses QueryPerformanceCounter on Windows
// and clock_gettime(CLOCK_MONOTONIC) on POSIX
// Adapted from common public examples.
uint64_t get_time_ms(void);

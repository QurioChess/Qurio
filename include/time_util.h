#pragma once

#include <stdint.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <time.h>
#endif

uint64_t get_time_ms(void);

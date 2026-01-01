#pragma once

#include <time.h>

typedef struct
{
    clock_t start_time;
    clock_t time_to_search;
} TimeManagement;

void compute_time_to_search(TimeManagement *tm, clock_t start_time, clock_t time, clock_t inc, int movestogo);

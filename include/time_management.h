#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "time_util.h"

static const uint64_t TIME_MARGIN = 10;

typedef struct
{
    bool use_time_control;
    uint64_t end_time;
} TimeManagement;

void compute_time_to_search(TimeManagement *tm, uint64_t start_time, uint64_t time, uint64_t inc, int movestogo, bool use_time_control);
bool time_up(TimeManagement *tm);

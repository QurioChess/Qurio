#include "time_management.h"

void compute_time_to_search(TimeManagement *tm, uint64_t start_time, uint64_t time, uint64_t inc, int movestogo, bool use_time_control) {
    tm->use_time_control = use_time_control;
    movestogo = (movestogo == 0) ? 40 : movestogo;

    uint64_t theorical_end_time = start_time + (time / (uint64_t)movestogo) + inc;
    tm->end_time = (theorical_end_time > TIME_MARGIN) ? theorical_end_time - TIME_MARGIN : start_time + 1;
}

bool time_up(TimeManagement *tm) {
    if (!tm->use_time_control)
        return false;

    uint64_t current = get_time_ms();
    if (current >= tm->end_time)
        return true;
    return false;
}

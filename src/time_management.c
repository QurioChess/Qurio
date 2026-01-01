#include "time_management.h"

void compute_time_to_search(TimeManagement *tm, clock_t start_time, clock_t time, clock_t inc, int movestogo) {
    tm->start_time = start_time;
    movestogo = (movestogo == 0) ? 40 : movestogo;

    tm->time_to_search = (time / movestogo) + inc;
}

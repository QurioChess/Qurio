#include "search_engine.h"


void start_search(Position pos, SearchContext *search, int depth) {
    atomic_store_explicit(&search->stop, false, memory_order_relaxed);

    ThreadContext thread = { .nodes = 0 };
    Move bestmove;
    int value = negamax(pos, search, &thread, depth, &bestmove);

    if (value != -2147483648)
    {
        printf("info: nodes: %" PRIu64 "\n", thread.nodes);
        printf("bestmove "); print_move(bestmove);  printf("\n");
        fflush(stdout);
    }
    else
    {
        printf("Search did not succeed\n");
    }
}


void stop_search(SearchContext *search) {
    atomic_store_explicit(&search->stop, true, memory_order_relaxed);
}
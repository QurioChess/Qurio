#include "search_engine.h"


void start_search(Position pos, SearchContext *search, pthread_t *search_thread, int depth) {
    atomic_store_explicit(&search->stop, false, memory_order_relaxed);

    Move bestmove;
    int value;

    ThreadContext thread;

    thread.pos = pos;
    thread.search = search;
    thread.nodes = 0ULL;
    thread.best_move = &bestmove;
    thread.value = &value;
    thread.depth = depth;

    // iterative_deepening(&thread);
    pthread_create(search_thread, NULL, iterative_deepening, &thread);

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


void stop_search(SearchContext *search, pthread_t *search_thread) {
    atomic_store_explicit(&search->stop, true, memory_order_relaxed);
}
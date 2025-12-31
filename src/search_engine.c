#include "search_engine.h"


void start_search(Position pos, SearchContext *search, pthread_t *search_thread, int depth) {
    atomic_store_explicit(&search->stop, false, memory_order_relaxed);

    ThreadContext thread;

    thread.pos = pos;
    thread.search = search;
    thread.nodes = 0ULL;
    thread.best_move = (Move)0;
    thread.value = INVALID_SCORE;
    thread.depth = depth;
    thread.completed_depth = 0;

    pthread_create(search_thread, NULL, main_search, &thread);
}


void stop_search(SearchContext *search, pthread_t *search_thread) {
    atomic_store_explicit(&search->stop, true, memory_order_relaxed);
    pthread_join(*search_thread, NULL);
}
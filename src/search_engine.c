#include "search_engine.h"


void start_search(Position pos, SearchContext *search_ctx, pthread_t *search_thread, int depth) {
    atomic_store_explicit(&search_ctx->stop, false, memory_order_relaxed);

    ThreadContext thread_ctx;

    thread_ctx.pos = pos;
    thread_ctx.search_ctx = search_ctx;
    thread_ctx.nodes = 0ULL;
    thread_ctx.best_move = INVALID_MOVE;
    thread_ctx.score = INVALID_SCORE;
    thread_ctx.depth = depth;
    thread_ctx.completed_depth = 0;

    pthread_create(search_thread, NULL, main_search, &thread_ctx);
}


void stop_search(SearchContext *search_ctx, pthread_t *search_thread) {
    atomic_store_explicit(&search_ctx->stop, true, memory_order_relaxed);
    pthread_join(*search_thread, NULL);
}
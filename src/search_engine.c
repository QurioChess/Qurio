#include "search_engine.h"

void start_search(EngineState *engine, Depth depth) {
    atomic_store_explicit(&engine->search_ctx.stop, false, memory_order_relaxed);

    if (engine->search_running) {
        pthread_join(engine->search_threads[0], NULL);
        engine->search_running = false;
    }

    ThreadContext *thread_ctx = &engine->thread_ctxs[0];
    thread_ctx->pos = engine->pos;
    thread_ctx->search_ctx = &engine->search_ctx;
    thread_ctx->table = &engine->table;
    thread_ctx->nodes = 0ULL;
    thread_ctx->best_move = INVALID_MOVE;
    thread_ctx->score = INVALID_SCORE;
    thread_ctx->depth = depth;
    thread_ctx->completed_depth = 0;

    pthread_create(&engine->search_threads[0], NULL, main_search, thread_ctx);
    engine->search_running = true;
}

void stop_search(EngineState *engine) {
    if (!engine->search_running)
        return;

    atomic_store_explicit(&engine->search_ctx.stop, true, memory_order_relaxed);
    pthread_join(engine->search_threads[0], NULL);
    engine->search_running = false;
}

void wait_search(EngineState *engine) {
    if (!engine->search_running)
        return;

    pthread_join(engine->search_threads[0], NULL);
    engine->search_running = false;
}
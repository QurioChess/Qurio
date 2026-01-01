#pragma once

#include <inttypes.h>
#include <pthread.h>

#include "board.h"
#include "negamax.h"
#include "search_context.h"

#define MAX_THREADS 1

typedef struct {
    Position pos;
    SearchContext search_ctx;
    pthread_t search_threads[MAX_THREADS];
    ThreadContext thread_ctxs[MAX_THREADS];
} EngineState;

void start_search(EngineState *engine, int depth);
void stop_search(EngineState *engine);

#pragma once

#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>

#include "board.h"
#include "negamax.h"
#include "search_context.h"

#define MAX_THREADS 1

typedef struct
{
    Position pos;
    SearchContext search_ctx;
    pthread_t search_threads[MAX_THREADS];
    ThreadContext thread_ctxs[MAX_THREADS];
    bool search_running;
} EngineState;

void start_search(EngineState *engine, int depth);
void stop_search(EngineState *engine);
void wait_search(EngineState *engine);

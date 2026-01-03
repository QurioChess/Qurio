#pragma once

#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>

#include "board.h"
#include "negamax.h"
#include "search_context.h"
#include "transposition_table.h"

#define MAX_THREADS 1

typedef struct
{
    Position pos;
    SearchContext search_ctx;
    pthread_t search_threads[MAX_THREADS];
    ThreadContext thread_ctxs[MAX_THREADS];
    bool search_running;
    TT table;
} EngineState;

void start_search(EngineState *engine, Depth depth);
void stop_search(EngineState *engine);
void wait_search(EngineState *engine);

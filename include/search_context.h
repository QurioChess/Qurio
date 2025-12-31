#pragma once

#include <stdint.h>
#include <stdatomic.h>
#include <pthread.h>

#include "board.h"
#include "move.h"

typedef struct {
    atomic_bool stop;
} SearchContext;

typedef struct {
    Position pos;
    SearchContext *search_ctx;
    uint64_t nodes;
    Move best_move;
    Score score;
    int depth;
    int completed_depth;
} ThreadContext;

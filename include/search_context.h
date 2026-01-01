#pragma once

#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>

#include "board.h"
#include "move.h"
#include "time_management.h"

typedef struct
{
    atomic_bool stop;
    TimeManagement tm;
} SearchContext;

typedef struct
{
    Position pos;
    SearchContext *search_ctx;
    uint64_t nodes;
    Move best_move;
    Score score;
    int depth;
    int completed_depth;
} ThreadContext;

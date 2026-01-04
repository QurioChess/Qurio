#pragma once

#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>

#include "board.h"
#include "move.h"
#include "time_management.h"
#include "transposition_table.h"
#include "types.h"

typedef struct
{
    U64 hash_stack[MAX_GAME_LENGTH];
    HalfMove count;
} GameHistory;

typedef struct
{
    atomic_bool stop;
    TimeManagement tm;
} SearchContext;

typedef struct
{
    Position pos;
    SearchContext *search_ctx;
    GameHistory *history;
    TT *table;
    U64 nodes;
    Move best_move;
    Score score;
    Depth depth;
    Depth completed_depth;
} ThreadContext;

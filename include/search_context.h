#pragma once

#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>

#include "board.h"
#include "move.h"
#include "move_ordering.h"
#include "search_stats.h"
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

typedef struct {
    ButterflyHistory quiet_history;
} PersistentState;

typedef struct
{
    // Pointer to global ressources
    SearchContext *search_ctx;
    GameHistory *history;
    TT *table;

    // Persistent data (reset on ucinewgame)
    PersistentState persistent;

    // Search-Local data
    Position pos;
    uint64_t nodes;
    Move best_move;
    Score score;
    Depth depth;
    Depth completed_depth;

#ifdef SEARCH_STATS
    ThreadStats stats;
#endif
} ThreadContext;

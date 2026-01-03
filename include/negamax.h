#pragma once

#include <inttypes.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>

#include "board.h"
#include "evaluation.h"
#include "move.h"
#include "move_ordering.h"
#include "movegen.h"
#include "search_context.h"
#include "time_management.h"
#include "transposition_table.h"
#include "types.h"

typedef struct
{
    U64 hash_stack[MAX_DEPTH];
    int ply;
} SearchState;

Score negamax(Position pos, Score alpha, Score beta, Depth depth, SearchState *search_state, ThreadContext *thread_ctx, Move *pv_move);
Score quiescence(Position pos, Score alpha, Score beta, SearchState *search_state, ThreadContext *thread_ctx);
void *iterative_deepening(void *arg);
void *main_search(void *arg);
bool should_stop(ThreadContext *thread_ctx);
bool is_repetition(SearchState *search_state);

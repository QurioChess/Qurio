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

static const Depth ASPIRATION_WINDOW_DEPTH = 3;
static const Score ASPIRATION_WINDOW_HALF_DELTA = 25;

static const Depth RFP_DEPTH = 4;
static const Score RFP_DEPTH_SCALING = 100;

typedef struct
{
    U64 hash_stack[MAX_PLY];
    HalfMove ply;
} SearchState;

Score negamax(Position pos, Score alpha, Score beta, Depth depth, SearchState *search_state, ThreadContext *thread_ctx, Move *pv_move);
Score quiescence(Position pos, Score alpha, Score beta, SearchState *search_state, ThreadContext *thread_ctx);
Score aspiration_window(Score previousScore, Depth depth, SearchState *search_state, ThreadContext *thread_ctx, Move *pv_move);
void *iterative_deepening(void *arg);
void *main_search(void *arg);
bool should_stop(ThreadContext *thread_ctx);
bool is_repetition(SearchState *search_state, GameHistory *history, HalfMove limit);

void update_quiet_history(Position pos, Move move, Depth depth, ThreadContext *thread_ctx);

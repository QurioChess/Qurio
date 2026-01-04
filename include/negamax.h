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

Score negamax(Position pos, Score alpha, Score beta, Depth depth, int ply, ThreadContext *thread_ctx, Move *best_move);
Score quiescence(Position pos, Score alpha, Score beta, int ply, ThreadContext *thread_ctx);
void *iterative_deepening(void *arg);
void *main_search(void *arg);
bool should_stop(ThreadContext *thread_ctx);

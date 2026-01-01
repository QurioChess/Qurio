#pragma once

#include <inttypes.h>
#include <stdbool.h>

#include "types.h"
#include "board.h"
#include "movegen.h"
#include "move.h"
#include "evaluation.h"
#include "search_context.h"
#include "time_management.h"

Score negamax(Position pos, ThreadContext *thread, int depth, Move *best_move);
void* iterative_deepening(void *arg);
void* main_search(void *arg);
bool should_stop(ThreadContext *thread_ctx);

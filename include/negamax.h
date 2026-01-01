#pragma once

#include <inttypes.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>

#include "board.h"
#include "evaluation.h"
#include "move.h"
#include "movegen.h"
#include "search_context.h"
#include "time_management.h"
#include "types.h"

Score negamax(Position pos, ThreadContext *thread, int depth, Move *best_move);
void *iterative_deepening(void *arg);
void *main_search(void *arg);
bool should_stop(ThreadContext *thread_ctx);

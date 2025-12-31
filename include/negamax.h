#pragma once

#include <inttypes.h>

#include "board.h"
#include "movegen.h"
#include "move.h"
#include "evaluation.h"
#include "search_context.h"

int negamax(Position pos, ThreadContext *thread, int depth, Move *best_move);
void* iterative_deepening(void *arg);
void* main_search(void *arg);
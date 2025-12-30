#pragma once

#include "board.h"
#include "movegen.h"
#include "move.h"
#include "evaluation.h"
#include "search_context.h"

int negamax(Position pos, SearchContext *search, ThreadContext *thread, int depth, Move *best_move);
// int iterative_deepening(Position pos, int depth, Move *best_move);

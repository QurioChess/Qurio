#pragma once

#include <stdbool.h>

#include "board.h"
#include "move.h"
#include "movegen.h"

static const MoveScore PROMOTION_SCORE = 100;
static const MoveScore VICTIM_SCALING = 6;

void score_moves(Position pos, MoveList *move_list);
Move get_next_move(MoveList *move_list, int index);

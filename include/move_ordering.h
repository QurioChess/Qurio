#pragma once

#include <stdbool.h>

#include "board.h"
#include "move.h"
#include "movegen.h"


static const MoveScore TT_MOVE = 300000;
static const MoveScore PROMOTION_SCORE = 200000;
static const MoveScore CAPTURE_SCORE = 100000;

static const MoveScore VICTIM_SCALING = 6;

MoveScore mvv_lva(PieceType victim, PieceType attacker);

void score_moves(Position pos, MoveList *move_list, Move tt_move);
Move get_next_move(MoveList *move_list, int index);

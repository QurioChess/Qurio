#pragma once

#include <stdbool.h>

#include "board.h"
#include "move.h"
#include "movegen.h"
#include "types.h"

typedef MoveScore ButterflyHistory[2][NSQUARES][NSQUARES];

typedef struct {
    Move primary;
    Move secondary;
} KillerPair;

static const MoveScore TT_MOVE = 300000;
static const MoveScore PROMOTION_SCORE = 200000;
static const MoveScore CAPTURE_SCORE = 100000;
static const MoveScore MAX_QUIET_HISTORY = CAPTURE_SCORE - 5000;
static const MoveScore KILLER1 = CAPTURE_SCORE - 1000;
static const MoveScore KILLER2 = CAPTURE_SCORE - 2000;

static const MoveScore VICTIM_SCALING = 6;

MoveScore mvv_lva(PieceType victim, PieceType attacker);

void score_moves(Position pos, MoveList *move_list, Move tt_move, ButterflyHistory *quiet_history, KillerPair killers);
Move get_next_move(MoveList *move_list, int index);

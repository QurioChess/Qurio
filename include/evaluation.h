#pragma once

#include "board.h"

static const Score PAWN_VALUE = 100;
static const Score KNIGHT_VALUE = 300;
static const Score BISHOP_VALUE = 300;
static const Score ROOK_VALUE = 500;
static const Score QUEEN_VALUE = 900;

Score evaluate(Position pos);

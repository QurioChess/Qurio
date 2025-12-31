#pragma once

#include <stdint.h>
#include <stdio.h>

#include "types.h"

typedef uint16_t Move;

static const Move INVALID_MOVE = 0;

Move encode_move(Square from, Square to, PromotionType prom, MoveType type);
Square decode_move_from(Move move);
Square decode_move_to(Move move);
PromotionType decode_move_promotion(Move move);
MoveType decode_move_type(Move move);

void print_move(Move move);

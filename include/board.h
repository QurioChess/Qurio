#pragma once

#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>

#include "types.h"
#include "move.h"

typedef struct {
    U64 pieces[2][NPIECES];
    U64 occ[2];
    CastlingRight castling;
    Color side;
    Square enpassant;
} Position;

U64 bb_square_value(U64 bb, Square sq);
Piece get_piece_on(Position pos, Square sq);

void print_bitboard(U64 bb);
void print_position_bitboard(Position pos);
void print_position(Position pos);

void set_start_position(Position *pos);
void parse_fen(Position *pos, char *fen);

void make_move(Position *pos, Move move);
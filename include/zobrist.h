#pragma once

#include <stdint.h>

#include "types.h"

extern U64 PIECE_SQUARE_ZKEYS[12][64];
extern U64 CASTLING_ZKEYS[16];
extern U64 ENPASSANT_ZKEYS[8];
extern U64 SIDE_ZKEY;

void init_zobrist();
U64 next();

static inline U64 get_piece_square_zkey(Piece piece, Square sq) { return PIECE_SQUARE_ZKEYS[piece][sq]; }
static inline U64 get_castling_zkey(CastlingRight castling) { return CASTLING_ZKEYS[castling]; }
static inline U64 get_enpassant_zkey(int file) { return ENPASSANT_ZKEYS[file]; }
static inline U64 get_side_zkey() { return SIDE_ZKEY; }

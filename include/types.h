#pragma once

#include <stdint.h>
#include <limits.h>

static const int NRANKS = 8;
static const int NFILES = 8;

typedef uint64_t U64;

typedef uint8_t Color;
typedef uint8_t Square;
typedef uint8_t PieceType;
typedef uint8_t Piece;
typedef uint8_t PromotionType;
typedef uint8_t CastlingRight;
typedef uint8_t MoveType;
typedef int Score;

enum ColorConst { WHITE = 0, BLACK = 1 };
enum PieceTypeConst { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, NPIECES = 6 };
enum PieceConst { WPAWN, WKNIGHT, WBISHOP, WROOK, WQUEEN, WKING,  BPAWN, BKNIGHT, BBISHOP, BROOK, BQUEEN, BKING, NOPIECE = 255};
enum PromotionTypeConst { PROM_NONE, PROM_KNIGHT, PROM_BISHOP, PROM_ROOK, PROM_QUEEN };
enum MoveTypeConst { MOVE_DEFAULT, MOVE_PROM, MOVE_ENPASSANT, MOVE_CASTLING };
enum SquareConst {
    A1, B1, C1, D1, E1, F1, G1, H1, 
    A2, B2, C2, D2, E2, F2, G2, H2, 
    A3, B3, C3, D3, E3, F3, G3, H3, 
    A4, B4, C4, D4, E4, F4, G4, H4, 
    A5, B5, C5, D5, E5, F5, G5, H5, 
    A6, B6, C6, D6, E6, F6, G6, H6, 
    A7, B7, C7, D7, E7, F7, G7, H7, 
    A8, B8, C8, D8, E8, F8, G8, H8, 
    NOSQUARE = 255, NSQUARES = 64
};
enum CastlingRightConst { NO_CASTLING, WCASTLING_KING, WCASTLING_QUEEN, BCASTLING_KING = 4, BCASTLING_QUEEN = 8 };
enum ScoreConst { PAT_SCORE = 0, MATE_SCORE = INT_MAX - 1024, INVALID_SCORE = INT_MIN };

static const char * const PIECE_NAMES[] = { "P", "N", "B", "R", "Q", "K" };
static const char * const PROMOTION_PIECE_NAMES[] = { "-", "n", "b", "r", "q" };
static const char * const SQUARE_NAMES[] = { 
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", 
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2", 
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3", 
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4", 
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5", 
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6", 
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7", 
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8", 
};

static inline Square lsb_index(U64 bb) {
    Square idx = 0;
    while (!(bb & 1)) { bb >>= 1; idx++; }
    return idx;
}

static inline Square pop_lsb(U64 *bb) {
    Square idx = 0;
    U64 b = *bb;
    while (!(b & 1)) { b >>= 1; idx++; }
    *bb = *bb & (*bb - 1);
    return idx;
}

static inline int count_bits(U64 bb) {
    int count = 0;
    while (bb)
    {
        bb &= bb - 1;
        count++;
    }
    return count;    
}

static inline PieceType get_piece_type(Piece piece) {
    return piece % 6;
}

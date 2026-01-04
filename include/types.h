#pragma once

#include <limits.h>
#include <stdint.h>

static const int NRANKS = 8;
static const int NFILES = 8;
static const int MAX_DEPTH = 255;

typedef uint64_t U64;

static const U64 FILE_A = 0x0101010101010101ULL;
static const U64 FILE_B = FILE_A << 1;
static const U64 FILE_C = FILE_A << 2;
static const U64 FILE_D = FILE_A << 3;
static const U64 FILE_E = FILE_A << 4;
static const U64 FILE_F = FILE_A << 5;
static const U64 FILE_G = FILE_A << 6;
static const U64 FILE_H = FILE_A << 7;

static const U64 RANK_1 = 0x00000000000000FFULL;
static const U64 RANK_2 = RANK_1 << 8;
static const U64 RANK_3 = RANK_1 << 16;
static const U64 RANK_4 = RANK_1 << 24;
static const U64 RANK_5 = RANK_1 << 32;
static const U64 RANK_6 = RANK_1 << 40;
static const U64 RANK_7 = RANK_1 << 48;
static const U64 RANK_8 = RANK_1 << 56;

typedef uint8_t Color;
typedef uint8_t Square;
typedef uint8_t PieceType;
typedef uint8_t Piece;
typedef uint8_t PromotionType;
typedef uint8_t CastlingRight;
typedef uint8_t MoveType;
typedef int Score;
typedef int MoveScore;
typedef uint8_t Depth;

enum ColorConst {
    WHITE = 0,
    BLACK = 1
};
enum PieceTypeConst {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    NPIECES = 6
};
enum PieceConst {
    WPAWN,
    WKNIGHT,
    WBISHOP,
    WROOK,
    WQUEEN,
    WKING,
    BPAWN,
    BKNIGHT,
    BBISHOP,
    BROOK,
    BQUEEN,
    BKING,
    NOPIECE = 255
};
enum PromotionTypeConst {
    PROM_NONE,
    PROM_KNIGHT,
    PROM_BISHOP,
    PROM_ROOK,
    PROM_QUEEN
};
enum MoveTypeConst {
    MOVE_DEFAULT,
    MOVE_PROM,
    MOVE_ENPASSANT,
    MOVE_CASTLING
};
// clang-format off
enum SquareConst {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,
    NOSQUARE = 255,
    NSQUARES = 64
};
// clang-format on
enum CastlingRightConst {
    NO_CASTLING = 0b0000,
    WCASTLING_KING = 0b0001,
    WCASTLING_QUEEN = 0b0010,
    BCASTLING_KING = 0b0100,
    BCASTLING_QUEEN = 0b1000,
};
enum ScoreConst {
    PAT_SCORE = 0,
    MATE_SCORE = INT_MAX - 1024,
    INVALID_SCORE = INT_MIN
};

static const char *const PIECE_NAMES[] = {"P", "N", "B", "R", "Q", "K"};
static const char *const PROMOTION_PIECE_NAMES[] = {"-", "n", "b", "r", "q"};
// clang-format off
static const char *const SQUARE_NAMES[] = {
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
};
// clang-format on

static inline Square lsb_index(U64 bb) {
    return (Square)__builtin_ctzll(bb);
}

static inline Square pop_lsb(U64 *bb) {
    Square idx = lsb_index(*bb);
    *bb &= (*bb - 1);
    return idx;
}

static inline int count_bits(U64 bb) {
    return __builtin_popcountll(bb);
}

static inline PieceType get_piece_type(Piece piece) {
    return piece % 6;
}

static inline Piece get_piece(PieceType piece_type, Color side) {
    return (Piece)(piece_type + side * NPIECES);
}

static inline Square flip_square(Square sq) {
    return sq ^ 56;
}
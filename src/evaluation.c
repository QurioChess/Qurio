#include "evaluation.h"

Score evaluate(Position pos) {
    int wpawn_count = count_bits(pos.pieces[WHITE][PAWN]);
    int wknight_count = count_bits(pos.pieces[WHITE][KNIGHT]);
    int wbishop_count = count_bits(pos.pieces[WHITE][BISHOP]);
    int wrook_count = count_bits(pos.pieces[WHITE][ROOK]);
    int wqueen_count = count_bits(pos.pieces[WHITE][QUEEN]);

    int bpawn_count = count_bits(pos.pieces[BLACK][PAWN]);
    int bknight_count = count_bits(pos.pieces[BLACK][KNIGHT]);
    int bbishop_count = count_bits(pos.pieces[BLACK][BISHOP]);
    int brook_count = count_bits(pos.pieces[BLACK][ROOK]);
    int bqueen_count = count_bits(pos.pieces[BLACK][QUEEN]);

    int wvalue = PAWN_VALUE * wpawn_count + KNIGHT_VALUE * wknight_count + BISHOP_VALUE * wbishop_count + ROOK_VALUE * wrook_count + QUEEN_VALUE * wqueen_count;
    int bvalue = PAWN_VALUE * bpawn_count + KNIGHT_VALUE * bknight_count + BISHOP_VALUE * bbishop_count + ROOK_VALUE * brook_count + QUEEN_VALUE * bqueen_count;

    return (pos.side == WHITE) ? (wvalue - bvalue) : (bvalue - wvalue);
}
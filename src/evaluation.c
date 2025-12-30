#include "evaluation.h"

int evaluate(Position pos) {
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

    int wvalue = wpawn_count + 3 * (wknight_count + wbishop_count) + 5 * wrook_count + 9 * wqueen_count;
    int bvalue = bpawn_count + 3 * (bknight_count + bbishop_count) + 5 * brook_count + 9 * bqueen_count;

    return (pos.side == WHITE) ? (wvalue - bvalue) : (bvalue - wvalue);
}
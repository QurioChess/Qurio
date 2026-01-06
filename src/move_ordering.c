#include "move_ordering.h"

MoveScore mvv_lva(PieceType victim, PieceType attacker) {
    return VICTIM_SCALING * (MoveScore)victim - (MoveScore)attacker;
}

void score_moves(Position pos, MoveList *move_list, Move tt_move) {
    for (int i = 0; i < move_list->count; i++) {
        Move move = move_list->moves[i];

        if (move == tt_move) {
            move_list->scores[i] = TT_MOVE;
            continue;
        }

        Square from = decode_move_from(move);
        Square to = decode_move_to(move);
        PromotionType prom = decode_move_promotion(move);
        MoveFlags flags = classify_move(pos, move);

        move_list->scores[i] = 0;
        if ((flags & FLAG_PROMOTION)) {
            move_list->scores[i] += PROMOTION_SCORE + (MoveScore)prom;
        }

        if ((flags & FLAG_CAPTURE)) {
            PieceType attacker = get_piece_type(get_piece_on(pos, from));
            PieceType victim = get_piece_type(get_piece_on(pos, to));
            move_list->scores[i] += CAPTURE_SCORE + mvv_lva(victim, attacker);
        }

        if ((flags & FLAG_ENPASSANT)) {
            move_list->scores[i] += CAPTURE_SCORE;
        }

        if ((flags & FLAG_CASTLING)) {
        }

        if ((flags & FLAGS_QUIET)) {

        }
    }
}

Move get_next_move(MoveList *move_list, int index) {
    int best_index = index;
    for (int i = index + 1; i < move_list->count; i++) {
        if (move_list->scores[i] > move_list->scores[best_index]) {
            best_index = i;
        }
    }

    if (best_index != index) {
        Move temp_move = move_list->moves[index];
        move_list->moves[index] = move_list->moves[best_index];
        move_list->moves[best_index] = temp_move;

        MoveScore temp_score = move_list->scores[index];
        move_list->scores[index] = move_list->scores[best_index];
        move_list->scores[best_index] = temp_score;
    }

    return move_list->moves[index];
}

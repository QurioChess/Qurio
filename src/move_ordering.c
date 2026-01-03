#include "move_ordering.h"

void score_moves(Position pos, MoveList *move_list) {
    for (int i = 0; i < move_list->count; i++) {
        Move move = move_list->moves[i];
        Square from = decode_move_from(move);
        Square to = decode_move_to(move);
        PromotionType prom = decode_move_promotion(move);
        MoveType type = decode_move_type(move);

        Color stm = pos.side;
        Color op = stm ^ 1;

        bool is_capture = (pos.occ[op] & (1ULL << to));

        switch (type) {
        case MOVE_DEFAULT:
            if (is_capture) {
                PieceType attacker = get_piece_type(get_piece_on(pos, from));
                PieceType victim = get_piece_type(get_piece_on(pos, to));
                move_list->scores[i] = VICTIM_SCALING * (MoveScore)victim - (MoveScore)attacker;
            }
            break;
        case MOVE_PROM:
            move_list->scores[i] = PROMOTION_SCORE + (MoveScore)prom;
            if (is_capture) {
                PieceType victim = get_piece_type(get_piece_on(pos, to));
                move_list->scores[i] += VICTIM_SCALING * (MoveScore)victim;
            }
            break;
        case MOVE_ENPASSANT:
            move_list->scores[i] = 0;
            break;

        default:
            break;
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

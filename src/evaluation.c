#include "evaluation.h"

Score evaluate(Position pos) {
    Score midgame_score = 0;
    Score endgame_score = 0;
    int game_phase = 0;

    for (PieceType piece_type = 0; piece_type < NPIECES; piece_type++) {
        U64 white_bb = pos.pieces[WHITE][piece_type];
        while (white_bb) {
            Square sq = pop_lsb(&white_bb);

            midgame_score += MID_GAME_TABLES[piece_type][sq];
            endgame_score += END_GAME_TABLES[piece_type][sq];
            game_phase += PHASE_WEIGHTS[piece_type];
        }

        U64 black_bb = pos.pieces[BLACK][piece_type];
        while (black_bb) {
            Square sq = flip_square(pop_lsb(&black_bb));

            midgame_score -= MID_GAME_TABLES[piece_type][sq];
            endgame_score -= END_GAME_TABLES[piece_type][sq];
            game_phase += PHASE_WEIGHTS[piece_type];
        }
    }

    game_phase = (game_phase > 24) ? 24 : game_phase;
    Score weighted_score = (midgame_score * game_phase + endgame_score * (24 - game_phase)) / 24;

    return (pos.side == WHITE) ? weighted_score : -weighted_score;
}
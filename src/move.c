#include "move.h"


Move encode_move(Square from, Square to, PromotionType prom, MoveType type) {
    prom = (type == MOVE_PROM) ? (prom - 1) : 0;

    return (Move)(
        ((from & 0x3f))     |
        ((to & 0x3f) << 6)  |
        ((prom & 0x3) << 12)|
        (((type & 0x3) << 14))
    );
}

Square decode_move_from(Move move) {
    return (move & 0x3f);
}
Square decode_move_to(Move move) {
    return ((move >> 6) & 0x3f);
}
PromotionType decode_move_promotion(Move move) {
    if (decode_move_type(move) == MOVE_PROM)
    {
        return ((move >> 12) & 0x3) + 1;
    }
    return PROM_NONE;
}
MoveType decode_move_type(Move move) {
    return ((move >> 14) & 0x3);
}

void print_move(Move move) {
    Square from = decode_move_from(move);
    Square to = decode_move_to(move);
    PromotionType prom = decode_move_promotion(move);
    MoveType type = decode_move_type(move);

    switch (type)
    {
    case MOVE_DEFAULT:
        printf("%s%s", SQUARE_NAMES[from], SQUARE_NAMES[to]);
        break;
    case MOVE_PROM:
        printf("%s%s%s", SQUARE_NAMES[from], SQUARE_NAMES[to], PROMOTION_PIECE_NAMES[prom]);
        break;
    case MOVE_ENPASSANT:
        printf("%s%s", SQUARE_NAMES[from], SQUARE_NAMES[to]);
        // printf("ep:%s%s", SQUARE_NAMES[from], SQUARE_NAMES[to]);
        break;
    case MOVE_CASTLING:
        printf("%s%s", SQUARE_NAMES[from], SQUARE_NAMES[to]);
        // printf("castle:%s%s", SQUARE_NAMES[from], SQUARE_NAMES[to]);
        break;
    
    default:
        break;
    }
}
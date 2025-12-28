#include "movegen.h"

U64 generate_knight_attacks(Square sq) {
    U64 bb = (1ULL << sq);
    U64 northwest = ((bb & 0xfefefefefefeULL) << 15);
    U64 northeast = ((bb & 0x7f7f7f7f7f7fULL) << 17);
    U64 westnorth = ((bb & 0xfcfcfcfcfcfcfcULL) << 6);
    U64 westsouth = ((bb & 0xfcfcfcfcfcfcfc00ULL) >> 10);
    U64 eastnorth = ((bb & 0x3f3f3f3f3f3f3fULL) << 10);
    U64 eastsouth = ((bb & 0x3f3f3f3f3f3f3f00ULL) >> 6);
    U64 southwest = ((bb & 0xfefefefefefe0000ULL) >> 17);
    U64 southeast = ((bb & 0x7f7f7f7f7f7f0000ULL) >> 15);

    return northwest | northeast | westnorth | westsouth | eastnorth | eastsouth | southwest | southeast;
}

U64 generate_king_attacks(Square sq) {
    U64 bb = (1ULL << sq);
    U64 north = ((bb & 0xffffffffffffffULL) << 8);
    U64 northwest = ((bb & 0xfefefefefefefeULL) << 7);
    U64 northeast = ((bb & 0x7f7f7f7f7f7f7fULL) << 9);
    U64 west = ((bb & 0xfefefefefefefefeULL) >> 1);
    U64 east = ((bb & 0x7f7f7f7f7f7f7f7fULL) << 1);
    U64 south = ((bb & 0xffffffffffffff00ULL) >> 8);
    U64 southwest = ((bb & 0xfefefefefefefe00ULL) >> 9);
    U64 southeast = ((bb & 0x7f7f7f7f7f7f7f00ULL) >> 7);

    return north | northwest | northeast | west | east | south | southwest | southeast;
}

U64 generate_rook_attacks(Square sq, U64 occ) {
    U64 bb = (1ULL << sq);
    U64 attacks = 0ULL;
    int rank = sq / NFILES;
    int file = sq % NFILES;

    // north
    for (int i = 1; i < (NRANKS - rank); i++)
    {
        U64 target = (bb << (8*i));
        attacks = attacks | target;
        if ((target & occ) != 0) {
            break;
        }
    }
    // west
    for (int i = 1; i < file + 1; i++)
    {
        U64 target = (bb >> i);
        attacks = attacks | target;
        if ((target & occ) != 0) {
            break;
        }
    }
    // east
    for (int i = 1; i < (NFILES - file); i++)
    {
        U64 target = (bb << i);
        attacks = attacks | target;
        if ((target & occ) != 0) {
            break;
        }
    }
    // south
    for (int i = 1; i < rank + 1; i++)
    {
        U64 target = (bb >> (8*i));
        attacks = attacks | target;
        if ((target & occ) != 0) {
            break;
        }
    }
      
    return attacks;
}

U64 generate_bishop_attacks(Square sq, U64 occ) {
    U64 bb = (1ULL << sq);
    U64 attacks = 0ULL;
    int rank = sq / NFILES;
    int file = sq % NFILES;

    // norhthwest
    int len = (file < NRANKS - rank - 1) ? file : NRANKS - rank - 1;
    for (int i = 1; i < len + 1; i++)
    {
        U64 target = (bb << (7*i));
        attacks = attacks | target;
        if ((target & occ) != 0) {
            break;
        }
    }
    // norhtheast
    len = (NFILES - file - 1 < NRANKS - rank - 1) ? NFILES - file - 1 : NRANKS - rank - 1;
    for (int i = 1; i < len + 1; i++)
    {
        U64 target = (bb << (9*i));
        attacks = attacks | target;
        if ((target & occ) != 0) {
            break;
        }
    }
    // southwest
    len = (file < rank) ? file : rank;
    for (int i = 1; i < len + 1; i++)
    {
        U64 target = (bb >> (9*i));
        attacks = attacks | target;
        if ((target & occ) != 0) {
            break;
        }
    }
    // southeast
    len = (NFILES - file - 1 < rank) ? NFILES - file - 1: rank;
    for (int i = 1; i < len + 1; i++)
    {
        U64 target = (bb >> (7*i));
        attacks = attacks | target;
        if ((target & occ) != 0) {
            break;
        }
    }
      
    return attacks;
}

U64 generate_queen_attacks(Square sq, U64 occ) {
    return generate_rook_attacks(sq, occ) | generate_bishop_attacks(sq, occ);
}

U64 generate_pawn_attacks(Square sq, Color side) {
    U64 bb = (1ULL << sq);

    if (side == WHITE)
    {
        U64 northwest = ((bb & 0xfefefefefefefeULL) << 7);
        U64 northeast = ((bb & 0x7f7f7f7f7f7f7fULL) << 9);
        return northwest | northeast;
    }
    else
    {
        U64 southwest = ((bb & 0xfefefefefefefe00ULL) >> 9);
        U64 southeast = ((bb & 0x7f7f7f7f7f7f7f00ULL) >> 7);
        return southwest | southeast;
    }
}


bool is_square_attacked_by(Position pos, Square sq, Color side) {
    U64 occ = pos.occ[WHITE] | pos.occ[BLACK];

    U64 knight_attacks = generate_knight_attacks(sq);
    U64 bishop_attacks = generate_bishop_attacks(sq, occ);
    U64 rook_attacks = generate_rook_attacks(sq, occ);
    U64 queen_attacks = generate_queen_attacks(sq, occ);
    U64 king_attacks = generate_king_attacks(sq);
    U64 pawn_attacks = generate_pawn_attacks(sq, side ^ 1);

    if (knight_attacks & pos.pieces[side][KNIGHT])
    {
        return true;
    }
    if (bishop_attacks & pos.pieces[side][BISHOP])
    {
        return true;
    }
    if (rook_attacks & pos.pieces[side][ROOK])
    {
        return true;
    }
    if (queen_attacks & pos.pieces[side][QUEEN])
    {
        return true;
    }
    if (king_attacks & pos.pieces[side][KING])
    {
        return true;
    }
    if (pawn_attacks & pos.pieces[side][PAWN])
    {
        return true;
    }

    return false;
}


bool is_in_check(Position pos, Color side) {
    Square king_square = lsb_index(pos.pieces[side][KING]);
    Color op = side ^ 1;
    return is_square_attacked_by(pos, king_square, op);
}


void generate_knight_moves(U64 knight, U64 stm_occ, U64 op_occ, MoveList *move_list) {
    while (knight) {
        Square sq = pop_lsb(&knight);
        U64 knight_attacks = generate_knight_attacks(sq) & (~stm_occ);

        // capture
        U64 knight_captures = knight_attacks & op_occ;
        while (knight_captures)
        {
            Square target_sq = pop_lsb(&knight_captures);
            move_list->moves[move_list->count++] = encode_move(
                sq,
                target_sq,
                PROM_NONE,
                MOVE_DEFAULT
            );
        }
        // quiet
        U64 knight_quiets = knight_attacks & (~op_occ);
        while (knight_quiets)
        {
            Square target_sq = pop_lsb(&knight_quiets);
            move_list->moves[move_list->count++] = encode_move(
                sq,
                target_sq,
                PROM_NONE,
                MOVE_DEFAULT
            );
        }
    }
}

void generate_bishop_moves(U64 bishop, U64 stm_occ, U64 op_occ, MoveList *move_list) {
    U64 occ = stm_occ | op_occ;
    while (bishop) {
        Square sq = pop_lsb(&bishop);
        U64 bishop_attacks = generate_bishop_attacks(sq, occ) & (~stm_occ);

        // if (bishop_attacks) {
        //     printf("Bishop on: %s\n", SQUARE_NAMES[sq]);
        //     print_bitboard(bishop_attacks);
        // }

        // capture
        U64 bishop_captures = bishop_attacks & op_occ;
        while (bishop_captures)
        {
            Square target_sq = pop_lsb(&bishop_captures);
            move_list->moves[move_list->count++] = encode_move(
                sq,
                target_sq,
                PROM_NONE,
                MOVE_DEFAULT
            );
        }
        // quiet
        U64 bishop_quiets = bishop_attacks & (~op_occ);
        while (bishop_quiets)
        {
            Square target_sq = pop_lsb(&bishop_quiets);
            move_list->moves[move_list->count++] = encode_move(
                sq,
                target_sq,
                PROM_NONE,
                MOVE_DEFAULT
            );
        }
    }
}


void generate_rook_moves(U64 rook, U64 stm_occ, U64 op_occ, MoveList *move_list) {
    U64 occ = stm_occ | op_occ;
    while (rook) {
        Square sq = pop_lsb(&rook);
        U64 rook_attacks = generate_rook_attacks(sq, occ) & (~stm_occ);

        // capture
        U64 rook_captures = rook_attacks & op_occ;
        while (rook_captures)
        {
            Square target_sq = pop_lsb(&rook_captures);
            move_list->moves[move_list->count++] = encode_move(
                sq,
                target_sq,
                PROM_NONE,
                MOVE_DEFAULT
            );
        }
        // quiet
        U64 rook_quiets = rook_attacks & (~op_occ);
        while (rook_quiets)
        {
            Square target_sq = pop_lsb(&rook_quiets);
            move_list->moves[move_list->count++] = encode_move(
                sq,
                target_sq,
                PROM_NONE,
                MOVE_DEFAULT
            );
        }
    }
}

void generate_queen_moves(U64 queen, U64 stm_occ, U64 op_occ, MoveList *move_list) {
    U64 occ = stm_occ | op_occ;
    while (queen) {
        Square sq = pop_lsb(&queen);
        U64 queen_attacks = generate_queen_attacks(sq, occ) & (~stm_occ);

        // capture
        U64 queen_captures = queen_attacks & op_occ;
        while (queen_captures)
        {
            Square target_sq = pop_lsb(&queen_captures);
            move_list->moves[move_list->count++] = encode_move(
                sq,
                target_sq,
                PROM_NONE,
                MOVE_DEFAULT
            );
        }
        // quiet
        U64 queen_quiets = queen_attacks & (~op_occ);
        while (queen_quiets)
        {
            Square target_sq = pop_lsb(&queen_quiets);
            move_list->moves[move_list->count++] = encode_move(
                sq,
                target_sq,
                PROM_NONE,
                MOVE_DEFAULT
            );
        }
    }
}


void generate_pawn_moves(U64 pawn, Color stm, U64 stm_occ, U64 op_occ, Square enpassant, MoveList *move_list) {
    U64 occ = stm_occ | op_occ;

    U64 double_push_rank = (stm == WHITE) ? 0xff00ULL : 0xff000000000000ULL;
    U64 promotion_rank = (stm == WHITE) ? 0xff00000000000000ULL : 0xffULL;

    if (enpassant != NOSQUARE)
    {
        Color op = stm ^ 1;
        U64 enpassant_captures = generate_pawn_attacks(enpassant, op) & pawn;

        while (enpassant_captures)
        {
            Square from_sq = pop_lsb(&enpassant_captures);
            move_list->moves[move_list->count++] = encode_move(
                from_sq,
                enpassant,
                PROM_NONE,
                MOVE_ENPASSANT
            );
        }
    }

    while (pawn) {
        Square sq = pop_lsb(&pawn);
        U64 pawn_attacks = generate_pawn_attacks(sq, stm);

        // capture
        U64 pawn_captures = pawn_attacks & op_occ;
        while (pawn_captures)
        {
            Square target_sq = pop_lsb(&pawn_captures);
            
            if ((1ULL << target_sq) & promotion_rank) {
                move_list->moves[move_list->count++] = encode_move(
                    sq,
                    target_sq,
                    PROM_KNIGHT,
                    MOVE_PROM
                );
                move_list->moves[move_list->count++] = encode_move(
                    sq,
                    target_sq,
                    PROM_BISHOP,
                    MOVE_PROM
                );
                move_list->moves[move_list->count++] = encode_move(
                    sq,
                    target_sq,
                    PROM_ROOK,
                    MOVE_PROM
                );
                move_list->moves[move_list->count++] = encode_move(
                    sq,
                    target_sq,
                    PROM_QUEEN,
                    MOVE_PROM
                );
            }
            else {
                move_list->moves[move_list->count++] = encode_move(
                    sq,
                    target_sq,
                    PROM_NONE,
                    MOVE_DEFAULT
                );
            }
        }
        
        U64 bb = (1ULL << sq);
        U64 push = (stm == WHITE) ? ((bb << 8) & (~occ)) : ((bb >> 8) & (~occ));

        if (!push){
            continue;
        }
        
        Square target_sq = lsb_index(push);
        if ((1ULL << target_sq) & promotion_rank) {
            move_list->moves[move_list->count++] = encode_move(
                sq,
                target_sq,
                PROM_KNIGHT,
                MOVE_PROM
            );
            move_list->moves[move_list->count++] = encode_move(
                sq,
                target_sq,
                PROM_BISHOP,
                MOVE_PROM
            );
            move_list->moves[move_list->count++] = encode_move(
                sq,
                target_sq,
                PROM_ROOK,
                MOVE_PROM
            );
            move_list->moves[move_list->count++] = encode_move(
                sq,
                target_sq,
                PROM_QUEEN,
                MOVE_PROM
            );
        }
        else {
            move_list->moves[move_list->count++] = encode_move(
                sq,
                target_sq,
                PROM_NONE,
                MOVE_DEFAULT
            );
        }

        if (!(bb & double_push_rank)) {
            continue;
        }
        U64 double_push = (stm == WHITE) ? ((push << 8) & (~occ)) : ((push >> 8) & (~occ));

        if (!double_push){
            continue;
        }

        target_sq = lsb_index(double_push);
        move_list->moves[move_list->count++] = encode_move(
            sq,
            target_sq,
            PROM_NONE,
            MOVE_DEFAULT
        );
    }
}


void generate_king_moves(Square king_square, Color stm, U64 stm_occ, U64 op_occ, Position pos, MoveList *move_list) {
    Color op = stm ^ 1;
    U64 king_attacks = generate_king_attacks(king_square) & (~stm_occ);

    // capture
    U64 king_captures = king_attacks & op_occ;
    while (king_captures)
    {
        Square target_sq = pop_lsb(&king_captures);
        move_list->moves[move_list->count++] = encode_move(
            king_square,
            target_sq,
            PROM_NONE,
            MOVE_DEFAULT
        );
    }
    // quiet
    U64 king_quiets = king_attacks & (~op_occ);
    while (king_quiets)
    {
        Square target_sq = pop_lsb(&king_quiets);
        move_list->moves[move_list->count++] = encode_move(
            king_square,
            target_sq,
            PROM_NONE,
            MOVE_DEFAULT
        );
    }

    //castling
    if (is_square_attacked_by(pos, king_square, op))
    {   
        return;
    }

    CastlingRight castling = pos.castling;
    CastlingRight king_side = (stm == WHITE) ? WCASTLING_KING : BCASTLING_KING;
    CastlingRight queen_side = (stm == WHITE) ? WCASTLING_QUEEN : BCASTLING_QUEEN;
    U64 king_path = (stm == WHITE) ? 0x60ULL : 0x6000000000000000ULL;
    U64 queen_path = (stm == WHITE) ? 0xeULL : 0xe00000000000000ULL;
    Square king_castle_square = (stm == WHITE) ? G1 : G8;
    Square queen_castle_square = (stm == WHITE) ? C1 : C8;
    U64 occ = stm_occ | op_occ;

    if (castling & king_side)
    {
        if (!(occ & king_path))
        {
            Square fsquare = (stm == WHITE) ? F1 : F8;
            Square gsquare = (stm == WHITE) ? G1 : G8;
            
            if (!(is_square_attacked_by(pos, fsquare, op) || is_square_attacked_by(pos, gsquare, op)))
            {
                move_list->moves[move_list->count++] = encode_move(
                king_square,
                king_castle_square,
                PROM_NONE,
                MOVE_CASTLING
                );
            }
        }
    }

    if (castling & queen_side)
    {
        if (!(occ & queen_path))
        {
            Square dsquare = (stm == WHITE) ? D1 : D8;
            Square csquare = (stm == WHITE) ? C1 : C8;

            if (!(is_square_attacked_by(pos, dsquare, op) || is_square_attacked_by(pos, csquare, op)))
            {
                move_list->moves[move_list->count++] = encode_move(
                king_square,
                queen_castle_square,
                PROM_NONE,
                MOVE_CASTLING
                );
            }
        }
    }
}



void generate_pseudo_legals(Position pos, MoveList *move_list) {
    Color stm = pos.side;
    Color op = stm ^ 1;
    
    // pawn
    U64 pawn = pos.pieces[stm][PAWN];
    generate_pawn_moves(pawn, stm, pos.occ[stm], pos.occ[op], pos.enpassant, move_list);

    // knight
    U64 knight = pos.pieces[stm][KNIGHT];
    generate_knight_moves(knight, pos.occ[stm], pos.occ[op], move_list);

    // bishop
    U64 bishop = pos.pieces[stm][BISHOP];
    generate_bishop_moves(bishop, pos.occ[stm], pos.occ[op], move_list);

    // rook
    U64 rook = pos.pieces[stm][ROOK];
    generate_rook_moves(rook, pos.occ[stm], pos.occ[op], move_list);

    // queen
    U64 queen = pos.pieces[stm][QUEEN];
    generate_queen_moves(queen, pos.occ[stm], pos.occ[op], move_list);

    // king
    U64 king = pos.pieces[stm][KING];
    Square king_square = lsb_index(king);
    generate_king_moves(king_square, stm, pos.occ[stm], pos.occ[op], pos, move_list);

}


void print_move_list(MoveList move_list) {
    printf("Move List (%i):\n", move_list.count);
    for (int i = 0; i < move_list.count; i++)
    {
        printf("%i: ", i);
        print_move(move_list.moves[i]);
        printf("\n");
    }
}
#pragma once

#include <stdio.h>
#include <stdbool.h>

#include "board.h"
#include "move.h"

#define MAX_MOVES 255


typedef struct {
    Move moves[MAX_MOVES];
    int count;
} MoveList;



U64 generate_king_attacks(Square sq);
U64 generate_knight_attacks(Square sq);
U64 generate_rook_attacks(Square sq, U64 occ);
U64 generate_bishop_attacks(Square sq, U64 occ);
U64 generate_queen_attacks(Square sq, U64 occ);
U64 generate_pawn_attacks(Square sq, Color side);
bool is_square_attacked_by(Position pos, Square sq, Color side);
bool is_in_check(Position pos, Color side);


void generate_knight_moves(U64 knight, U64 stm_occ, U64 op_occ, MoveList *move_list);
void generate_bishop_moves(U64 bishop, U64 stm_occ, U64 op_occ, MoveList *move_list);
void generate_rook_moves(U64 rook, U64 stm_occ, U64 op_occ, MoveList *move_list);
void generate_queen_moves(U64 queen, U64 stm_occ, U64 op_occ, MoveList *move_list);
void generate_pawn_moves(U64 pawn, Color stm, U64 stm_occ, U64 op_occ, Square enpassant, MoveList *move_list);
void generate_king_moves(Square king_square, Color stm, U64 stm_occ, U64 op_occ, Position pos, MoveList *move_list);

void generate_pseudo_legals(Position pos, MoveList *move_list);

void print_move_list(MoveList move_list);
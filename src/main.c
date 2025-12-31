#include <stdio.h>
#include <stddef.h>
#include <time.h>

#include "board.h"
#include "movegen.h"
#include "move.h"
#include "search_engine.h"
#include "uci.h"

U64 perft(Position pos, int depth) {
    MoveList move_list = { .count = 0 };
    U64 nodes = 0ULL;
    
    if (depth == 0) return 1ULL;

    generate_pseudo_legals(pos, &move_list);
    for (int i = 0; i < move_list.count; i++)
    {
        Position next_pos = pos;
        make_move(&next_pos, move_list.moves[i]);
        if (is_in_check(next_pos, next_pos.side ^ 1)) continue;
        nodes += perft(next_pos, depth - 1);             
    }
    return nodes; 
}


void divide_perft(Position pos, int depth)  {
    clock_t start = clock();

    MoveList move_list = { .count = 0 };
    U64 nodes = 0ULL;

    generate_pseudo_legals(pos, &move_list);
    for (int i = 0; i < move_list.count; i++)
    {
        Position next_pos = pos;
        make_move(&next_pos, move_list.moves[i]);
        if (is_in_check(next_pos, next_pos.side ^ 1)) continue;
        
        U64 subnodes = perft(next_pos, depth - 1);
        print_move(move_list.moves[i]);
        printf(": %" PRIu64 "\n", subnodes);
        nodes += subnodes;

    }

    clock_t end = clock();
    double time_taken_sec = (double)(end - start) / CLOCKS_PER_SEC; 
    double time_taken_ms = time_taken_sec * 1000.0;

    printf("Number of nodes: %" PRIu64 "\n", nodes);
    printf("Time taken: %.3f ms\n", time_taken_ms);

    if (time_taken_ms > 0) {
        double nps = (double)nodes / (time_taken_ms / 1000.0); // nodes per second
        printf("NPS: %.0f\n", nps);
    } else {
        printf("Time too short to calculate NPS accurately.\n");
    }
}




int main() {
    printf("Qurio - Chess engine\n");
    print_bitboard(MATE_SCORE);


    // printf("Size of Position struct: %zu bytes\n", sizeof(Position));

    // printf("Offset of pieces: %zu bytes\n", offsetof(Position, pieces));
    // printf("Offset of occ: %zu bytes\n", offsetof(Position, occ));
    // printf("Offset of side: %zu bytes\n", offsetof(Position, side));
    // printf("Offset of castling: %zu bytes\n", offsetof(Position, castling));

    // Position pos;
    // set_start_position(&pos);
    // print_position_bitboard(pos);

    // printf("Piece on A1 (0): %i\n", get_piece_on(pos, 0));
    // printf("Piece on A2 (1): %i\n", get_piece_on(pos, 1));
    // printf("Piece on A3 (2): %i\n", get_piece_on(pos, 2));
    // printf("Piece on A4 (3): %i\n", get_piece_on(pos, 3));
    // printf("Piece on A5 (4): %i\n", get_piece_on(pos, 4));
    // printf("Piece on A6 (5): %i\n", get_piece_on(pos, 5));
    // printf("Piece on A7 (6): %i\n", get_piece_on(pos, 6));
    // printf("Piece on A8 (7): %i\n", get_piece_on(pos, 7));

    // print_bitboard(pos.pieces[0][0]);

    // for (int i = 0; i < 64; i++)
    // {
    //     printf("Square: %i -> value: %i\n", i, bb_square_value(pos.pieces[0][0], i));
    // }
    
    // print_position(pos);

    // MoveList move_list = { .count = 0 };
    // generate_pseudo_legals(pos, &move_list);

    // make_move(&pos, move_list.moves[0]);
    // printf("Going down to move: ");
    // print_move(move_list.moves[0]);
    // printf("\n");

    // print_position(pos);


    // move_list.count = 0;
    // generate_pseudo_legals(pos, &move_list);

    // make_move(&pos, move_list.moves[5]);
    // printf("Going down to move: ");
    // print_move(move_list.moves[5]);
    // printf("\n");

    // print_position(pos);

    // move_list.count = 0;
    // generate_pseudo_legals(pos, &move_list);

    // make_move(&pos, move_list.moves[18]);
    // printf("Going down to move: ");
    // print_move(move_list.moves[18]);
    // printf("\n");

    // print_position(pos);

    // move_list.count = 0;
    // generate_pseudo_legals(pos, &move_list);

    // make_move(&pos, move_list.moves[21]);
    // printf("Going down to move: ");
    // print_move(move_list.moves[21]);
    // printf("\n");

    // print_position(pos);

    // move_list.count = 0;
    // generate_pseudo_legals(pos, &move_list);

    // make_move(&pos, move_list.moves[12]);
    // printf("Going down to move: ");
    // print_move(move_list.moves[12]);
    // printf("\n");

    // print_position(pos);


    // printf("Is in check?: %i", is_in_check(pos, pos.side ^ 1));
    // divide_perft(pos, 7);



    // Square from = H8;
    // Square to = A6;
    // PromotionType prom = PROM_KNIGHT;
    // bool capture = true;

    // printf("From: ");
    // printBits(from);
    // printf("To: ");
    // printBits(to);
    // printf("Prom: ");
    // printBits(prom);
    // printf("Capture: ");
    // printBits(capture);
    // printf("Encode: ");
    // printBits(encode_move(from, to, prom, capture));
    // print_move(encode_move(from, to, prom, capture));

    // Color side = WHITE;
    // printBits(side);
    // printBits(side ^ 1);


    // U64 occ = 0x0ULL;
    // Square sq = C7;

    // print_bitboard(occ);
    // print_bitboard(generate_bishop_attacks(sq, occ));

    // U64 occ = 0x0ULL;

    // for (Square i = A1; i < NSQUARES; i++)
    // {
    //     printf("Square %s:\n", SQUARE_NAMES[i]);
    //     print_bitboard(generate_bishop_attacks(i, occ));
    // }

    // printf("Size of ULL: %zu\n", sizeof(1ULL));
    // printf("Size of uint64_t: %zu\n", sizeof((uint64_t)1ULL));
    // printf("1 << 0: %" PRIu64 "\n", (1ULL<<0));
    // printf("1 << 1: %" PRIu64 "\n", (1ULL<<1));
    // printf("1 << 16: %" PRIu64 "\n", (1ULL<<16));
    // printf("1 << 32: %" PRIu64 "\n", (1ULL<<32));
    // printf("1 << 62: %" PRIu64 "\n", (1ULL<<62));
    // printf("1 << 63: %" PRIu64 "\n", (1ULL<<63));

    // parse_fen(&pos, "8/7P/8/7K/2B5/8/8/k7 w - - 0 1");
    // print_position(pos);


    // MoveList move_list = { .count = 0 };
    // generate_pseudo_legals(pos, &move_list);

    // make_move(&pos, move_list.moves[2]);
    // printf("Going down to move: ");
    // print_move(move_list.moves[2]);
    // printf("\n");
    // print_position(pos);

    // move_list.count = 0;
    // generate_pseudo_legals(pos, &move_list);

    // make_move(&pos, move_list.moves[0]);
    // printf("Going down to move: ");
    // print_move(move_list.moves[0]);
    // printf("\n");
    // print_position(pos);

    // move_list.count = 0;
    // generate_pseudo_legals(pos, &move_list);

    // make_move(&pos, move_list.moves[16]);
    // printf("Going down to move: ");
    // print_move(move_list.moves[16]);
    // printf("\n");
    // print_position(pos);

    // move_list.count = 0;
    // generate_pseudo_legals(pos, &move_list);

    // make_move(&pos, move_list.moves[0]);
    // printf("Going down to move: ");
    // print_move(move_list.moves[0]);
    // printf("\n");
    // print_position(pos);

    // printf("\n\n");

    // printf("Is in check?: %i", is_in_check(pos, pos.side ^ 1));

    // divide_perft(pos, 1);

    // parse_fen(&pos, "8/7p/2p4P/2P5/N7/5pp1/r1k1p3/4K3 w - - 2 52");

    // divide_perft(pos, 6);


    // Move best_move;
    // iterative_deepening(pos, 6, &best_move);
    // printf("Best move: ");
    // print_move(best_move);

    // search(pos, 6);

    // main_loop();

    return 0;
}

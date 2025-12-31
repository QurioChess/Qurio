#include "perft.h"

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
        double nps = (double)nodes / (time_taken_ms / 1000.0);
        printf("NPS: %.0f\n", nps);
    } else {
        printf("Time too short to calculate NPS accurately.\n");
    }
}
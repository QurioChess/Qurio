#include "perft.h"

U64 perft(Position pos, int depth) {
    MoveList move_list = {.count = 0};
    U64 nodes = 0ULL;

    if (depth == 0)
        return 1ULL;

    generate_pseudo_legals(pos, &move_list);
    for (int i = 0; i < move_list.count; i++) {
        Position next_pos = pos;
        make_move(&next_pos, move_list.moves[i]);
        if (is_in_check(next_pos, next_pos.side ^ 1))
            continue;
        nodes += perft(next_pos, depth - 1);
    }
    return nodes;
}

void divide_perft(Position pos, int depth) {
    uint64_t start = get_time_ms();

    MoveList move_list = {.count = 0};
    U64 nodes = 0ULL;

    generate_pseudo_legals(pos, &move_list);
    for (int i = 0; i < move_list.count; i++) {
        Position next_pos = pos;
        make_move(&next_pos, move_list.moves[i]);
        if (is_in_check(next_pos, next_pos.side ^ 1))
            continue;

        U64 subnodes = perft(next_pos, depth - 1);
        print_move(move_list.moves[i]);
        printf(": %" PRIu64 "\n", subnodes);
        nodes += subnodes;
    }

    uint64_t end = get_time_ms();
    uint64_t time_taken_ms = end - start;
    time_taken_ms = (time_taken_ms > 0) ? time_taken_ms : 1;
    double time_taken_s = (double)time_taken_ms / 1000.0;

    // Human-friendly output
    printf("\n=== Perft Summary ===\n");
    printf("Total nodes searched: %" PRIu64 "\n", nodes);
    printf("Time taken          : %" PRIu64 " ms (%.3f s)\n", time_taken_ms, time_taken_s);
    printf("Nodes per second    : %" PRIu64 "\n", (uint64_t)((double)nodes / time_taken_s));
}

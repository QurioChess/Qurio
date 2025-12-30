#include "search_engine.h"


void search(Position pos, int depth) {
    SearchContext search = { .stop = false };


    ThreadContext thread = { .nodes = 0 };
    Move bestmove;
    int value = negamax(pos, &search, &thread, depth, &bestmove);

    if (value != -2147483648)
    {
        printf("info: nodes: %" PRIu64 "\n", thread.nodes);
        printf("bestmove "); print_move(bestmove);  printf("\n");
        fflush(stdout);
    }
    else
    {
        printf("Search did not succeed\n");
    }
}
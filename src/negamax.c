#include "negamax.h"

int negamax(Position pos, ThreadContext *thread, int depth, Move *best_move)
{
    if (atomic_load_explicit(&thread->search->stop, memory_order_relaxed))
    {
        return -2147483648;
    }
    thread->nodes++;

    if (depth == 0) return evaluate(pos);
    
    int legal_moves_count = 0;
    MoveList move_list = { .count = 0 };
    generate_pseudo_legals(pos, &move_list);

    int best_value = -100000;
    for (int i = 0; i < move_list.count; i++)
    {
        Position next_pos = pos;
        make_move(&next_pos, move_list.moves[i]);
        if (is_in_check(next_pos, next_pos.side ^ 1)) continue;
        legal_moves_count++;

        int value = -negamax(next_pos, thread, depth - 1, NULL);

        if (value > best_value)
        {
            best_value = value;
            if (best_move != NULL)
            {
                *best_move = move_list.moves[i];
            }
            
        }
    }

    if (legal_moves_count == 0) {
        if (is_in_check(pos, pos.side))
        {
            return -10000;
        }
        else
        {
            return 0;
        }
    }

    return best_value;
}


void* iterative_deepening(void *arg) {
    ThreadContext *thread = (ThreadContext*) arg;
    for (int d = 1; d < thread->depth + 1; d++)
    {
        Move current_best;
        int current_value = negamax(thread->pos, thread, d, &current_best);
        printf("Search at (depth: %i) (nodes: %lu) (value: %i): ", d, thread->nodes, current_value); print_move(current_best); printf("\n");

        if ((current_value != -2147483648) && (!atomic_load_explicit(&thread->search->stop, memory_order_relaxed))) {
            thread->best_move = current_best;
            thread->value = current_value;
            thread->completed_depth = d;
        }
    }
    return NULL;
}


void* main_search(void *arg) {
    iterative_deepening(arg);

    ThreadContext *thread = (ThreadContext*) arg;
    if (thread->value != -2147483648)
    {
        printf("info: nodes: %" PRIu64 "\n", thread->nodes);
        printf("info: completed_depth: %i\n", thread->completed_depth);
        printf("bestmove "); print_move(thread->best_move);  printf("\n");
        fflush(stdout);
    }
    else
    {
        printf("Search did not succeed\n");
    }

    return NULL;
}

#include "negamax.h"

bool should_stop(ThreadContext *thread_ctx) {
    if (atomic_load_explicit(&thread_ctx->search_ctx->stop, memory_order_relaxed))
        return true;
    if (((thread_ctx->nodes % 2048) == 0) && time_up(&thread_ctx->search_ctx->tm)) {
        atomic_store_explicit(&thread_ctx->search_ctx->stop, true, memory_order_relaxed);
    }
    return false;
}

Score negamax(Position pos, Score alpha, Score beta, int depth, int ply, ThreadContext *thread_ctx, Move *best_move) {
    if (should_stop(thread_ctx))
        return INVALID_SCORE;

    thread_ctx->nodes++;
    if (depth == 0)
        return evaluate(pos);

    int legal_moves_count = 0;
    MoveList move_list = {.count = 0};
    generate_pseudo_legals(pos, &move_list);

    int best_value = -MATE_SCORE;
    for (int i = 0; i < move_list.count; i++) {
        Position next_pos = pos;
        make_move(&next_pos, move_list.moves[i]);
        if (is_in_check(next_pos, next_pos.side ^ 1))
            continue;
        legal_moves_count++;

        int value = -negamax(next_pos, -beta, -alpha, depth - 1, ply + 1, thread_ctx, NULL);

        if (value > best_value) {
            best_value = (value > MATE_SCORE) ? MATE_SCORE : value;
            if (best_move != NULL) {
                *best_move = move_list.moves[i];
            }
        }

        if (value > alpha) {
            alpha = value;

            if (alpha >= beta)
                break;
        }
    }

    if (legal_moves_count == 0) {
        if (is_in_check(pos, pos.side)) {
            return -(MATE_SCORE - ply);
        } else {
            return PAT_SCORE;
        }
    }

    return best_value;
}

void *iterative_deepening(void *arg) {
    ThreadContext *thread_ctx = (ThreadContext *)arg;
    for (int d = 1; d < thread_ctx->depth + 1; d++) {
        Move current_best;
        Score current_score = negamax(thread_ctx->pos, -MATE_SCORE, +MATE_SCORE, d, 0, thread_ctx, &current_best);

        if ((current_score == INVALID_SCORE) || (atomic_load_explicit(&thread_ctx->search_ctx->stop, memory_order_relaxed)))
            break;

        thread_ctx->best_move = current_best;
        thread_ctx->score = current_score;
        thread_ctx->completed_depth = d;

        printf("info depth %d nodes %" PRIu64 " ", d, thread_ctx->nodes);

        if (current_score > MATE_SCORE_BOUNDARY) {
            printf("score mate %d ", MATE_SCORE - current_score);
        } else if (current_score < -MATE_SCORE_BOUNDARY) {
            printf("score mate %d ", -(MATE_SCORE + current_score));
        } else {
            printf("score cp %d ", current_score);
        }

        printf("pv ");
        print_move(current_best);
        printf("\n");

    }
    return NULL;
}

void *main_search(void *arg) {
    iterative_deepening(arg);

    ThreadContext *thread_ctx = (ThreadContext *)arg;
    if (thread_ctx->score != INVALID_SCORE) {
        printf("bestmove ");
        print_move(thread_ctx->best_move);
        printf("\n");
        fflush(stdout);
    } else {
        printf("Search did not succeed\n");
    }

    return NULL;
}

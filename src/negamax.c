#include "negamax.h"

bool should_stop(ThreadContext *thread_ctx) {
    if (atomic_load_explicit(&thread_ctx->search_ctx->stop, memory_order_relaxed))
        return true;
    if (((thread_ctx->nodes % 2048) == 0) && time_up(&thread_ctx->search_ctx->tm)) {
        atomic_store_explicit(&thread_ctx->search_ctx->stop, true, memory_order_relaxed);
    }
    return false;
}

bool is_repetition(SearchState *search_state, GameHistory *history, HalfMove limit) {
    int current_ply = search_state->ply;
    if (current_ply == 0)
        return false;

    if (limit >= 100)
        return true;

    U64 current_hash = search_state->hash_stack[current_ply];
    HalfMove total_half_moves = history->count + current_ply;

    for (HalfMove dist = 2; (dist < total_half_moves) && (dist <= limit); dist += 2) {
        if (dist <= current_ply) {
            if (search_state->hash_stack[current_ply - dist] == current_hash)
                return true;
        } else {
            HalfMove history_dist = dist - current_ply;
            int history_index = (history->count - 1) - history_dist;
            if ((history_index >= 0) && (history->hash_stack[history_index] == current_hash))
                return true;
        }
    }

    return false;
}

Score negamax(Position pos, Score alpha, Score beta, Depth depth, SearchState *search_state, ThreadContext *thread_ctx, Move *pv_move) {
    if (should_stop(thread_ctx))
        return INVALID_SCORE;
    thread_ctx->nodes++;

    if (is_repetition(search_state, thread_ctx->history, pos.halfmove_clock)) {
        return DRAW_SCORE;
    }

    TTEntry *entry = probe_tt(thread_ctx->table, pos.hash);
    Move tt_move = INVALID_MOVE;
    if (entry != NULL) {
        if ((search_state->ply != 0) && (entry->depth >= depth)) {
            if (entry->type == EXACT)
                return entry->score;
            if ((entry->type == LOWER_BOUND) && (entry->score >= beta))
                return entry->score;
            if ((entry->type == UPPER_BOUND) && (entry->score <= alpha))
                return entry->score;
        }
        tt_move = entry->best_move;
    }

    if (depth == 0)
        return quiescence(pos, alpha, beta, search_state, thread_ctx);

    int legal_moves_count = 0;
    MoveList move_list = {.count = 0};
    generate_pseudo_legals(pos, &move_list, false);
    score_moves(pos, &move_list, tt_move);

    Score best_value = -MATE_SCORE;
    Score initial_alpha = alpha;
    Move best_move;

    for (int i = 0; i < move_list.count; i++) {
        Position next_pos = pos;
        Move move = get_next_move(&move_list, i);
        make_move(&next_pos, move);
        if (is_in_check(next_pos, next_pos.side ^ 1))
            continue;
        legal_moves_count++;

        search_state->hash_stack[++search_state->ply] = next_pos.hash;
        int value = -negamax(next_pos, -beta, -alpha, depth - 1, search_state, thread_ctx, NULL);
        search_state->ply--;

        if (value > best_value) {
            best_value = (value > MATE_SCORE) ? MATE_SCORE : value;
            best_move = move;
            if (pv_move != NULL) {
                *pv_move = move;
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
            return -MATE_SCORE;
        } else {
            return DRAW_SCORE;
        }
    }

    if (!should_stop(thread_ctx)) {
        EntryType type = (best_value >= beta) ? UPPER_BOUND : (best_value > initial_alpha) ? EXACT
                                                                                           : LOWER_BOUND;
        store_tt(thread_ctx->table, pos.hash, depth, best_move, best_value, type);
    }

    return best_value;
}

Score quiescence(Position pos, Score alpha, Score beta, SearchState *search_state, ThreadContext *thread_ctx) {
    if (should_stop(thread_ctx))
        return INVALID_SCORE;
    thread_ctx->nodes++;

    TTEntry *entry = probe_tt(thread_ctx->table, pos.hash);
    Move tt_move = INVALID_MOVE;
    if (entry != NULL) {
        if (entry->type == EXACT)
            return entry->score;
        if ((entry->type == LOWER_BOUND) && (entry->score >= beta))
            return entry->score;
        if ((entry->type == UPPER_BOUND) && (entry->score <= alpha))
            return entry->score;
        tt_move = entry->best_move;
    }

    Score static_eval = evaluate(pos);

    if (static_eval >= beta)
        return static_eval;
    if (static_eval > alpha)
        alpha = static_eval;

    if (search_state->ply > MAX_DEPTH)
        return static_eval;

    MoveList move_list = {.count = 0};
    generate_pseudo_legals(pos, &move_list, true);
    score_moves(pos, &move_list, tt_move);

    Score best_value = static_eval;
    for (int i = 0; i < move_list.count; i++) {
        Position next_pos = pos;
        Move move = get_next_move(&move_list, i);

        make_move(&next_pos, move);
        if (is_in_check(next_pos, next_pos.side ^ 1))
            continue;

        search_state->hash_stack[++search_state->ply] = next_pos.hash;
        int value = -quiescence(next_pos, -beta, -alpha, search_state, thread_ctx);
        search_state->ply--;

        if (value > best_value) {
            best_value = (value > MATE_SCORE) ? MATE_SCORE : value;
        }

        if (value > alpha) {
            alpha = value;

            if (alpha >= beta)
                break;
        }
    }

    return best_value;
}

void *iterative_deepening(void *arg) {
    ThreadContext *thread_ctx = (ThreadContext *)arg;
    SearchState search_state;
    for (Depth d = 1;; d++) {
        Move current_best;
        search_state.ply = 0;
        search_state.hash_stack[0] = thread_ctx->pos.hash;

        Score current_score = negamax(thread_ctx->pos, -MATE_SCORE, +MATE_SCORE, d, &search_state, thread_ctx, &current_best);
        printf("Search at (depth: %" PRIu8 ") (nodes: %" PRIu64 ") (value: %i): ", d, thread_ctx->nodes, current_score);
        print_move(current_best);
        printf("\n");

        if ((current_score == INVALID_SCORE) || (atomic_load_explicit(&thread_ctx->search_ctx->stop, memory_order_relaxed)))
            break;

        thread_ctx->best_move = current_best;
        thread_ctx->score = current_score;
        thread_ctx->completed_depth = d;

        if (d == thread_ctx->depth)
            break;
    }
    return NULL;
}

void *main_search(void *arg) {
    iterative_deepening(arg);

    ThreadContext *thread_ctx = (ThreadContext *)arg;
    if (thread_ctx->score != INVALID_SCORE) {
        printf("info: nodes: %" PRIu64 "\n", thread_ctx->nodes);
        printf("info: completed_depth: %i\n", thread_ctx->completed_depth);
        printf("bestmove ");
        print_move(thread_ctx->best_move);
        printf("\n");
        fflush(stdout);
    } else {
        printf("Search did not succeed\n");
    }

    return NULL;
}

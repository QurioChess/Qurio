#include "negamax.h"

Depth LMR_REDUCTION_TABLE[MAX_DEPTH][MAX_MOVES];
int LMP_TABLE[MAX_DEPTH];

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

void update_quiet_ordering(Position pos, Move move, Depth depth, ThreadContext *thread_ctx, SearchState *search_state) {
    MoveFlags flags = classify_move(pos, move);
    if (!(flags & (FLAG_CAPTURE | FLAG_ENPASSANT | FLAG_PROMOTION))) {
        Color stm = pos.side;
        Square from = decode_move_from(move);
        Square to = decode_move_to(move);
        thread_ctx->persistent.quiet_history[stm][from][to] += (MoveScore)depth * (MoveScore)depth;

        if (thread_ctx->persistent.quiet_history[stm][from][to] > MAX_QUIET_HISTORY) {
            thread_ctx->persistent.quiet_history[stm][from][to] = MAX_QUIET_HISTORY;
        }

        if (search_state->killers[search_state->ply].primary != move) {
            search_state->killers[search_state->ply].secondary = search_state->killers[search_state->ply].primary;
            search_state->killers[search_state->ply].primary = move;
        }
    }
}

Score negamax(Position pos, Score alpha, Score beta, Depth depth, SearchState *search_state, ThreadContext *thread_ctx, Move *pv_move) {
    if (should_stop(thread_ctx))
        return INVALID_SCORE;
    thread_ctx->nodes++;

    if (search_state->ply >= MAX_PLY - 1) {
        return evaluate(pos);
    }

    if (is_repetition(search_state, thread_ctx->history, pos.halfmove_clock)) {
        return DRAW_SCORE;
    }

    TTEntry *entry = probe_tt(thread_ctx->table, pos.hash);
    Move tt_move = INVALID_MOVE;
    if (entry != NULL) {
        if ((search_state->ply != 0) && (entry->depth >= depth)) {
            Score tt_score = score_from_tt(entry->score, search_state->ply);
            if (entry->type == EXACT)
                return tt_score;
            if ((entry->type == LOWER_BOUND) && (tt_score >= beta))
                return tt_score;
            if ((entry->type == UPPER_BOUND) && (tt_score <= alpha))
                return tt_score;
        }
        tt_move = entry->best_move;
    }

    if (depth == 0)
        return quiescence(pos, alpha, beta, search_state, thread_ctx);

    search_state->killers[search_state->ply + 1].primary = INVALID_MOVE;
    search_state->killers[search_state->ply + 1].secondary = INVALID_MOVE;

    bool is_root = search_state->ply == 0;
    bool is_pv = is_root || (beta - alpha > 1);

    if (!is_pv) {
        bool is_check = is_in_check(pos, pos.side);
        if (!is_check) {
            // Reverse futility pruning
            Score static_eval = evaluate(pos);
            if ((depth < RFP_DEPTH) && (static_eval >= beta + RFP_DEPTH_SCALING * (Score)depth)) {
                return static_eval;
            }

            // Null move pruning
            if (!search_state->last_move_is_null && has_non_pawn(pos, pos.side)) {
                Depth depth_reduction = (depth > NMP_DEPTH_REDUCTION) ? NMP_DEPTH_REDUCTION : depth;

                Position next_pos = pos;
                make_null_move(&next_pos);

                search_state->hash_stack[++search_state->ply] = next_pos.hash;
                search_state->last_move_is_null = true;

                Score nmp_value = -negamax(next_pos, -beta, -beta + 1, depth - depth_reduction, search_state, thread_ctx, NULL);

                search_state->ply--;
                search_state->last_move_is_null = false;

                if (nmp_value >= beta)
                    return nmp_value;
            }
        }
    }

    int legal_moves_count = 0;
    int legal_quiet_count = 0;
    bool skip_quiet = false;
    MoveList move_list = {.count = 0};
    generate_pseudo_legals(pos, &move_list, false);
    score_moves(pos, &move_list, tt_move, &thread_ctx->persistent.quiet_history, search_state->killers[search_state->ply]);

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

        MoveFlags flags = classify_move(pos, move);
        bool is_quiet = !(flags & (FLAG_CAPTURE | FLAG_ENPASSANT | FLAG_PROMOTION));
        legal_quiet_count++;

        if (!skip_quiet && (legal_quiet_count > LMP_TABLE[depth])) {
            skip_quiet = true;
        }

        if (skip_quiet && is_quiet) {
            continue;            
        }

        search_state->hash_stack[++search_state->ply] = next_pos.hash;
        search_state->last_move_is_null = false;
        int value = INVALID_SCORE;

        Depth reduction = 0;
        if ((legal_moves_count > LMR_MOVE_COUNT) && (depth >= LMR_DEPTH)) {
            reduction = LMR_REDUCTION_TABLE[depth][legal_moves_count];
        }

        // First move: full search
        if (legal_moves_count == 1) {
            value = -negamax(next_pos, -beta, -alpha, depth - 1, search_state, thread_ctx, NULL);
        } else {
            // zw search
            value = -negamax(next_pos, -alpha - 1, -alpha, depth - 1 - reduction, search_state, thread_ctx, NULL);

            if ((reduction > 0) && (value > alpha)) {
                value = -negamax(next_pos, -alpha - 1, -alpha, depth - 1, search_state, thread_ctx, NULL);
            }

            if (value > alpha && value < beta) {
                // research full window
                value = -negamax(next_pos, -beta, -alpha, depth - 1, search_state, thread_ctx, NULL);
            }
        }
        search_state->ply--;

        if (value > best_value) {
            best_value = value;
            best_move = move;
            if (pv_move != NULL) {
                *pv_move = move;
            }
        }

        if (value > alpha) {
            alpha = value;

            if (alpha >= beta) {
                update_quiet_ordering(pos, move, depth, thread_ctx, search_state);
                break;
            }
        }
    }

    if (legal_moves_count == 0) {
        if (is_in_check(pos, pos.side)) {
            return -(MATE_SCORE - search_state->ply);
        } else {
            return DRAW_SCORE;
        }
    }

    if (!should_stop(thread_ctx)) {
        EntryType type = (best_value >= beta) ? LOWER_BOUND : (best_value > initial_alpha) ? EXACT
                                                                                           : UPPER_BOUND;
        store_tt(thread_ctx->table, pos.hash, depth, best_move, score_to_tt(best_value, search_state->ply), type);
    }

    return best_value;
}

Score quiescence(Position pos, Score alpha, Score beta, SearchState *search_state, ThreadContext *thread_ctx) {
    if (should_stop(thread_ctx))
        return INVALID_SCORE;
    thread_ctx->nodes++;

    if (search_state->ply >= MAX_PLY - 1) {
        return evaluate(pos);
    }

    TTEntry *entry = probe_tt(thread_ctx->table, pos.hash);
    Move tt_move = INVALID_MOVE;
    if (entry != NULL) {
        Score tt_score = score_from_tt(entry->score, search_state->ply);
        if (entry->type == EXACT)
            return tt_score;
        if ((entry->type == LOWER_BOUND) && (tt_score >= beta))
            return tt_score;
        if ((entry->type == UPPER_BOUND) && (tt_score <= alpha))
            return tt_score;
        tt_move = entry->best_move;
    }

    Score static_eval = evaluate(pos);

    if (static_eval >= beta)
        return static_eval;
    if (static_eval > alpha)
        alpha = static_eval;

    MoveList move_list = {.count = 0};
    generate_pseudo_legals(pos, &move_list, true);
    score_moves(pos, &move_list, tt_move, &thread_ctx->persistent.quiet_history, search_state->killers[search_state->ply]);

    Score best_value = static_eval;
    for (int i = 0; i < move_list.count; i++) {
        Position next_pos = pos;
        Move move = get_next_move(&move_list, i);

        make_move(&next_pos, move);
        if (is_in_check(next_pos, next_pos.side ^ 1))
            continue;

        search_state->hash_stack[++search_state->ply] = next_pos.hash;
        search_state->last_move_is_null = false;
        int value = -quiescence(next_pos, -beta, -alpha, search_state, thread_ctx);
        search_state->ply--;

        if (value > best_value) {
            best_value = value;
        }

        if (value > alpha) {
            alpha = value;

            if (alpha >= beta)
                break;
        }
    }

    return best_value;
}

Score aspiration_window(Score previousScore, Depth depth, SearchState *search_state, ThreadContext *thread_ctx, Move *pv_move) {
    if (depth < ASPIRATION_WINDOW_DEPTH) {
        search_state->ply = 0;
        search_state->hash_stack[0] = thread_ctx->pos.hash;
        search_state->last_move_is_null = false;
        return negamax(thread_ctx->pos, -ALPHA_BETA_BOUND, +ALPHA_BETA_BOUND, depth, search_state, thread_ctx, pv_move);
    }

    Score alpha = previousScore - ASPIRATION_WINDOW_HALF_DELTA;
    Score beta = previousScore + ASPIRATION_WINDOW_HALF_DELTA;
    Score delta = beta - alpha;
    Score current_score = INVALID_SCORE;

    for (;;) {
        alpha = (alpha < -ALPHA_BETA_BOUND) ? -ALPHA_BETA_BOUND : alpha;
        beta = (beta > ALPHA_BETA_BOUND) ? ALPHA_BETA_BOUND : beta;

        search_state->ply = 0;
        search_state->hash_stack[0] = thread_ctx->pos.hash;
        search_state->last_move_is_null = false;
        current_score = negamax(thread_ctx->pos, alpha, beta, depth, search_state, thread_ctx, pv_move);

        if ((current_score == INVALID_SCORE) || (atomic_load_explicit(&thread_ctx->search_ctx->stop, memory_order_relaxed)))
            break;

        // Aspiration window in bound search is valid
        if ((alpha < current_score) && (current_score < beta))
            break;

        delta = delta * 2;
        // fail-low (score <= alpha): decrease alpha
        if (current_score <= alpha) {

            alpha = beta - delta;
        }
        // fail-high (beta <= score): increase beta
        else {
            beta = alpha + delta;
        }
    }

    return current_score;
}

void *iterative_deepening(void *arg) {
    ThreadContext *thread_ctx = (ThreadContext *)arg;
    SearchState search_state;
    Score current_score = INVALID_SCORE;

    search_state.killers[0].primary = INVALID_MOVE;
    search_state.killers[0].secondary = INVALID_MOVE;

    for (Depth d = 1;; d++) {
        Move current_best;
        current_score = aspiration_window(current_score, d, &search_state, thread_ctx, &current_best);

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

        if (d == thread_ctx->depth)
            break;
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

void init_lmr_table() {
    for (int d = 0; d < MAX_DEPTH; d++) {
        for (int m = 0; m < MAX_MOVES; m++) {
            if ((d == 0) || (m == 0)) {
                LMR_REDUCTION_TABLE[d][m] = 0;
                continue;
            }
            double theorical_reduction = LMR_BASE_REDUCTION + LMR_SCALING * log(d) * log(m);
            int reduction = (int)theorical_reduction;

            // next_depth = d - reduction - 1 >= 0
            // reduction <= d - 1
            if (reduction >= d) {
                reduction = d - 1;
            }

            LMR_REDUCTION_TABLE[d][m] = (Depth)reduction;
        }
    }
}

void init_lmp_table() {
    for (int d = 0; d < MAX_DEPTH; d++) {
        double theorical_threshold = LMP_CONSTANT + LMP_LINEAR * d + LMP_QUADRATIC * d * d;
        LMP_TABLE[d] = (int)theorical_threshold;
    }
}

void init_negamax_tables() {
    init_lmr_table();
    init_lmp_table();
}
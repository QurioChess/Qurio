#include "search_stats.h"
#include <inttypes.h>
#include <stdio.h>

#ifdef SEARCH_STATS

void init_search_stats(SearchStats *stats) {
    reset_search_stats(stats);
}

void reset_search_stats(SearchStats *stats) {
    stats->tt_probes = 0;
    stats->tt_hits = 0;
    stats->tt_cutoffs = 0;
    stats->tt_stores = 0;

    stats->rfp_applied = 0;
    stats->rfp_cutoffs = 0;
    stats->nmp_applied = 0;
    stats->nmp_cutoffs = 0;
    stats->lmr_applied = 0;
    stats->lmr_failed_high = 0;
    stats->pvs_failed_high = 0;

    stats->killer_primary_hits = 0;
    stats->killer_secondary_hits = 0;
    stats->history_overflow = 0;

    stats->alpha_cutoffs = 0;
    stats->beta_cutoffs = 0;
    stats->first_move_cutoffs = 0;

    stats->aspiration_failed = 0;

    stats->negamax_nodes = 0;
    stats->quiescience_nodes = 0;
    stats->negamax_moves = 0;
    stats->quiescience_moves = 0;
}

void merge_search_stats(SearchStats *dest, const SearchStats *source) {
    dest->tt_probes += source->tt_probes;
    dest->tt_hits += source->tt_hits;
    dest->tt_cutoffs += source->tt_cutoffs;
    dest->tt_stores += source->tt_stores;

    dest->rfp_applied += source->rfp_applied;
    dest->rfp_cutoffs += source->rfp_cutoffs;
    dest->nmp_applied += source->nmp_applied;
    dest->nmp_cutoffs += source->nmp_cutoffs;
    dest->lmr_applied += source->lmr_applied;
    dest->lmr_failed_high += source->lmr_failed_high;
    dest->pvs_failed_high += source->pvs_failed_high;

    dest->killer_primary_hits += source->killer_primary_hits;
    dest->killer_secondary_hits += source->killer_secondary_hits;
    dest->history_overflow += source->history_overflow;

    dest->alpha_cutoffs += source->alpha_cutoffs;
    dest->beta_cutoffs += source->beta_cutoffs;
    dest->first_move_cutoffs += source->first_move_cutoffs;

    dest->aspiration_failed += source->aspiration_failed;

    dest->negamax_nodes += source->negamax_nodes;
    dest->quiescience_nodes += source->quiescience_nodes;
    dest->negamax_moves += source->negamax_moves;
    dest->quiescience_moves += source->quiescience_moves;
}

void print_iteration_stats(SearchStats *stats, Depth depth) {
    uint64_t killer_hits = stats->killer_primary_hits + stats->killer_secondary_hits;
    double negamax_branch_factor = stats->negamax_nodes > 0
                                       ? (double)stats->negamax_moves / (double)stats->negamax_nodes
                                       : 0.0;
    double quiescience_branch_factor = stats->quiescience_nodes > 0
                                           ? (double)stats->quiescience_moves / (double)stats->quiescience_nodes
                                           : 0.0;
    double branch_factor = (stats->negamax_nodes + stats->quiescience_nodes) > 0
                               ? (double)(stats->negamax_moves + stats->quiescience_moves) / (double)(stats->negamax_nodes + stats->quiescience_nodes)
                               : 0.0;

    printf("stats depth=%d ", depth);
    printf("tt_probes=%" PRIu64 " tt_hits=%" PRIu64 " tt_cutoffs=%" PRIu64 " tt_stores=%" PRIu64 " ",
           stats->tt_probes, stats->tt_hits, stats->tt_cutoffs, stats->tt_stores);
    printf("rfp_applied=%" PRIu64 " rfp_cutoffs=%" PRIu64 " ",
           stats->rfp_applied, stats->rfp_cutoffs);
    printf("nmp_applied=%" PRIu64 " nmp_cutoffs=%" PRIu64 " ",
           stats->nmp_applied, stats->nmp_cutoffs);
    printf("lmr_applied=%" PRIu64 " lmr_failed=%" PRIu64 " ",
           stats->lmr_applied, stats->lmr_failed_high);
    printf("pvs_failed_high=%" PRIu64 " aspiration_failed=%" PRIu64 " ",
           stats->pvs_failed_high, stats->aspiration_failed);
    printf("killer_hits=%" PRIu64 " history_overflow=%" PRIu64 " ",
           killer_hits, stats->history_overflow);
    printf("alpha_cutoffs=%" PRIu64 " beta_cutoffs=%" PRIu64 " first_move_cutoffs=%" PRIu64 " ",
           stats->alpha_cutoffs, stats->beta_cutoffs, stats->first_move_cutoffs);
    printf("negamax_branch_factor=%.2f, quiescience_branch_factor=%.2f, branch_factor=%.2f\n", negamax_branch_factor, quiescience_branch_factor, branch_factor);
}

void print_final_stats(ThreadStats *thread_stats, Depth completed_depth, uint64_t total_nodes) {
    SearchStats *cum = &thread_stats->cumulative_stats;

    uint64_t total_killer_hits = cum->killer_primary_hits + cum->killer_secondary_hits;
    double tt_hit_rate = cum->tt_probes > 0 ? (100.0 * (double)cum->tt_hits) / (double)cum->tt_probes : 0.0;
    double tt_cutoff_rate = cum->tt_hits > 0 ? (100.0 * (double)cum->tt_cutoffs) / (double)cum->tt_hits : 0.0;
    double negamax_branch_factor = cum->negamax_nodes > 0
                                       ? (double)cum->negamax_moves / (double)cum->negamax_nodes
                                       : 0.0;
    double quiescience_branch_factor = cum->quiescience_nodes > 0
                                           ? (double)cum->quiescience_moves / (double)cum->quiescience_nodes
                                           : 0.0;
    double branch_factor = (cum->negamax_nodes + cum->quiescience_nodes) > 0
                               ? (double)(cum->negamax_moves + cum->quiescience_moves) / (double)(cum->negamax_nodes + cum->quiescience_nodes)
                               : 0.0;

    printf("stats_summary depth=%d total_nodes=%" PRIu64 "\n", completed_depth, total_nodes);
    printf("  hash_table: probes=%" PRIu64 " hits=%" PRIu64 " cutoffs=%" PRIu64 " stores=%" PRIu64 " hit_rate=%.1f%% cutoff_rate=%.1f%%\n",
           cum->tt_probes, cum->tt_hits, cum->tt_cutoffs, cum->tt_stores, tt_hit_rate, tt_cutoff_rate);
    printf("  pruning: nmp_cutoffs=%" PRIu64 " rfp_cutoffs=%" PRIu64 " lmr_failed=%" PRIu64 " pvs_failed=%" PRIu64 "\n",
           cum->nmp_cutoffs, cum->rfp_cutoffs, cum->lmr_failed_high, cum->pvs_failed_high);
    printf("  move_ordering: killer_hits=%" PRIu64 "\n",
           total_killer_hits);
    printf("  cutoffs: beta=%" PRIu64 " alpha=%" PRIu64 " first_move=%" PRIu64 "\n",
           cum->beta_cutoffs, cum->alpha_cutoffs, cum->first_move_cutoffs);
    printf("  negamax branching_factor: %.2f, quiescience branching_factor: %.2f, branching_factor: %.2f\n", negamax_branch_factor, quiescience_branch_factor, branch_factor);
}

#endif

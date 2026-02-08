#pragma once

#include "types.h"
#include <stdint.h>

#ifdef SEARCH_STATS

typedef struct {
    // Hash Table Statistics
    uint64_t tt_probes;  // Total TT lookups
    uint64_t tt_hits;    // Successful TT probes
    uint64_t tt_cutoffs; // Successful TT cutoffs
    uint64_t tt_stores;  // TT entries stored

    // Pruning Statistics
    uint64_t rfp_applied;     // Reverse Futility Pruning tried
    uint64_t rfp_cutoffs;     // RFP cutoffs
    uint64_t nmp_applied;     // Null Move Pruning applied
    uint64_t nmp_cutoffs;     // NMP cutoffs
    uint64_t lmr_applied;     // Late Move Reduction applied
    uint64_t lmr_failed_high; // LMR re-searched after fail-high
    uint64_t pvs_failed_high; // PVS re-searched after fail-high

    // Move Ordering Statistics
    uint64_t killer_primary_hits;   // Primary killer move matched
    uint64_t killer_secondary_hits; // Secondary killer move matched
    uint64_t history_overflow;      // History heuristic clipped

    // Cutoff Statistics
    uint64_t alpha_cutoffs;      // Moves that improved alpha
    uint64_t beta_cutoffs;       // Moves that caused beta cutoff
    uint64_t first_move_cutoffs; // Cutoffs by first move

    // Aspiration window Statistics
    uint64_t aspiration_failed;

    // Branching Analysis
    uint64_t negamax_nodes;     // Negamax Nodes
    uint64_t quiescience_nodes; // Quiescience Nodes
    uint64_t negamax_moves;     // Negamax moves
    uint64_t quiescience_moves; // Quiescience moves

} SearchStats;

typedef struct {
    SearchStats iteration_stats;
    SearchStats cumulative_stats;
} ThreadStats;

void init_search_stats(SearchStats *stats);
void reset_search_stats(SearchStats *stats);
void merge_search_stats(SearchStats *dest, const SearchStats *source);
void print_iteration_stats(SearchStats *stats, Depth depth);
void print_final_stats(ThreadStats *thread_stats, Depth completed_depth, uint64_t total_nodes);

#endif

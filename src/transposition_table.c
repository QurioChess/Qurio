#include "transposition_table.h"

TTEntry *probe_tt(TT *table, U64 hash) {
    size_t index = hash % table->size;
    TTEntry *entry = &table->underlying[index];

    if (entry->hash == hash)
        return entry;
    return NULL;
}

void store_tt(TT *table, U64 hash, Depth depth, Move best_move, Score score, EntryType type) {
    size_t index = hash % table->size;

    table->underlying[index].hash = hash;
    table->underlying[index].best_move = best_move;
    table->underlying[index].depth = depth;
    table->underlying[index].score = score;
    table->underlying[index].type = type;
}

Score score_from_tt(Score tt_score, HalfMove ply) {
    if (tt_score > MATE_SCORE_BOUNDARY)
        return tt_score - ply;
    else if (tt_score < -MATE_SCORE_BOUNDARY)
        return tt_score + ply;
    else
        return tt_score;
}

Score score_to_tt(Score score, HalfMove ply) {
    if (score > MATE_SCORE_BOUNDARY)
        return score + ply;
    else if (score < -MATE_SCORE_BOUNDARY)
        return score - ply;
    else
        return score;
}

void init_tt(TT *table, size_t megabytes) {
    table->underlying = NULL;
    table->size = 0;
    resize_tt(table, megabytes);
}

void resize_tt(TT *table, size_t megabytes) {
    size_t bytes = megabytes * 1024 * 1024;
    size_t new_size = bytes / sizeof(TTEntry);
    TTEntry *new_entries = malloc(sizeof(TTEntry) * new_size);
    
    if (new_entries == NULL) {
        printf("info string Error: Could not allocate %zu MB for TT\n", megabytes);
        return;
    }

    if (table->underlying) {
        free(table->underlying);
    }

    table->underlying = new_entries;
    table->size = new_size;

    printf("info string TT set to %zu entries (%zu MB)\n", new_size, megabytes);
    clear_tt(table);
}

void clear_tt(TT *table) {
    for (size_t i = 0; i < table->size; i++) {
        table->underlying[i].hash = 0;
    }
}

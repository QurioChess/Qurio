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
    size_t bytes = megabytes * 1024 * 1024;
    size_t size = bytes / sizeof(TTEntry);

    printf(">>> Creating tt with: %zu entries\n", size);
    table->underlying = malloc(sizeof(TTEntry) * size);
    table->size = size;

    clear_tt(table);
}

void clear_tt(TT *table) {
    for (size_t i = 0; i < table->size; i++) {
        table->underlying[i].hash = 0;
    }
}

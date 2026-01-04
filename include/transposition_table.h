#pragma once

#include <stdint.h>
#include <stdlib.h>

#include "move.h"
#include "types.h"

typedef uint8_t EntryType;

enum EntryTypeConst {
    EXACT,
    UPPER_BOUND,
    LOWER_BOUND,
};

typedef struct
{
    U64 hash;
    Score score;
    Move best_move;
    EntryType type;
    Depth depth;
} TTEntry;

typedef struct
{
    TTEntry *underlying;
    size_t size;
} TT;

TTEntry *probe_tt(TT *table, U64 hash);
void store_tt(TT *table, U64 hash, Depth depth, Move best_move, Score score, EntryType type);
Score score_from_tt(Score tt_score, HalfMove ply);
Score score_to_tt(Score score, HalfMove ply);

void init_tt(TT *table, size_t megabytes);
void clear_tt(TT *table);

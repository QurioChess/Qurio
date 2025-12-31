#pragma once

#include <stdint.h>
#include <stdatomic.h>
#include <pthread.h>

#include "move.h"

typedef struct {
    atomic_bool stop;
} SearchContext;

typedef struct {
    Position pos;
    SearchContext *search;
    uint64_t nodes;
    Move *best_move;
    int *value;
    int depth;
} ThreadContext;

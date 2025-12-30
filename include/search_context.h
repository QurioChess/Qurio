#pragma once

#include <stdint.h>
#include <stdatomic.h>

typedef struct {
    atomic_bool stop;
} SearchContext;

typedef struct {
    uint64_t nodes;
} ThreadContext;

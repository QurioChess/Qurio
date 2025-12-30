#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    volatile bool stop;
} SearchContext;

typedef struct {
    uint64_t nodes;
} ThreadContext;

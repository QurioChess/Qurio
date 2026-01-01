#pragma once

#include <stdio.h>
#include <stdint.h>

#include "types.h"
#include "board.h"
#include "movegen.h"
#include "time_util.h"

U64 perft(Position pos, int depth);
void divide_perft(Position pos, int depth);

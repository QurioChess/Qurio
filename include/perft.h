#pragma once

#include <stdio.h>
#include <time.h>

#include "types.h"
#include "board.h"
#include "movegen.h"

U64 perft(Position pos, int depth);
void divide_perft(Position pos, int depth);
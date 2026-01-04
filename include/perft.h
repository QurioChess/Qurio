#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "movegen.h"
#include "time_util.h"
#include "types.h"

U64 perft(Position pos, int depth);
void divide_perft(Position pos, int depth);

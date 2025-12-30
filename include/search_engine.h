#pragma once

#include <inttypes.h>

#include "board.h"
#include "negamax.h"
#include "search_context.h"


void start_search(Position pos, SearchContext *search, int depth);
void stop_search(SearchContext *search);
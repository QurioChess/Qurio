#pragma once

#include <inttypes.h>
#include <pthread.h>

#include "board.h"
#include "negamax.h"
#include "search_context.h"


void start_search(Position pos, SearchContext *search, pthread_t *search_thread, int depth);
void stop_search(SearchContext *search, pthread_t *search_thread);
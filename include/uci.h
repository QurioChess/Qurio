#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>

#include "board.h"
#include "move.h"
#include "movegen.h"
#include "search_engine.h"
#include "perft.h"
#include "time_management.h"
#include "time_util.h"

/*
go wtime <> btime <> winc <> binc <> movestogo <>
position startpos
position fen <fen>
quit
stop
uci
ucinewgame
isready
*/

void main_loop();

void command_position(Position *pos, char* position_options);
Move parse_move(Position pos, char* move);
void command_go(Position pos, SearchContext *search_ctx, pthread_t *search_thread, char* go_options);
void command_stop(SearchContext *search_ctx, pthread_t *search_thread);
void command_uci();
void command_ucinewgame(Position *pos);
void command_isready();

void command_perft(Position pos, char *perft_options);
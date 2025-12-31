#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "board.h"
#include "move.h"
#include "movegen.h"
#include "search_engine.h"

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
void command_go(Position pos, SearchContext *search, pthread_t *search_thread, char* go_options);
void command_stop(SearchContext *search, pthread_t *search_thread);
void command_uci();
void command_ucinewgame(Position *pos);
void command_isready();

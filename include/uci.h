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

void command_position(EngineState *engine, char* position_options);
Move parse_move(Position pos, char* move);
void command_go(EngineState *engine, char* go_options);
void command_stop(EngineState *engine);
void command_uci();
void command_ucinewgame(EngineState *engine);
void command_isready();

void command_perft(EngineState *engine, char *perft_options);
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "move.h"
#include "movegen.h"
#include "perft.h"
#include "search_engine.h"
#include "time_management.h"
#include "time_util.h"
#include "types.h"

void main_loop();

void command_position(EngineState *engine, char *position_options);
Move parse_move(Position pos, char *move);
void command_go(EngineState *engine, char *go_options);
void command_stop(EngineState *engine);
void command_uci();
void command_ucinewgame(EngineState *engine);
void command_isready();

void command_perft(EngineState *engine, char *perft_options);

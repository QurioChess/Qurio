#include "uci.h"

void command_position(EngineState *engine, char *position_options) {
    Position *pos = &engine->pos;
    GameHistory *history = &engine->history;
    reset_history(history);

    if (strncmp(position_options, "fen", 3) == 0) {
        parse_fen(pos, position_options + 4);
    } else {
        set_start_position(pos);
    }
    push_hash(history, pos->hash);

    char *moves = strstr(position_options, "moves");
    if (moves == NULL)
        return;
    moves += 6;

    char *move_token = strtok(moves, " ");
    while (move_token != NULL) {
        Move move = parse_move(*pos, move_token);
        if (move == INVALID_MOVE) {
            printf("Invalid move: '%s', Current position:\n", move_token);
            print_position(*pos);
            MoveList move_list = {.count = 0};
            generate_pseudo_legals(*pos, &move_list, false);
            printf("Available moves: \n");
            print_move_list(move_list);
            return;
        }

        make_move(pos, move);
        push_hash(history, pos->hash);

        move_token = strtok(NULL, " ");
    }
}

Move parse_move(Position pos, char *move_token) {
    MoveList move_list = {.count = 0};
    generate_pseudo_legals(pos, &move_list, false);

    int from_file = move_token[0] - 'a';
    int from_rank = (move_token[1] - '1');
    int to_file = move_token[2] - 'a';
    int to_rank = (move_token[3] - '1');

    Square from = (Square)((from_rank * NFILES) + from_file);
    Square to = (Square)((to_rank * NFILES) + to_file);

    for (int i = 0; i < move_list.count; i++) {
        Move candidate = move_list.moves[i];
        if ((from == decode_move_from(candidate)) && (to == decode_move_to(candidate))) {
            if (decode_move_type(candidate) != MOVE_PROM)
                return candidate;

            if (strlen(move_token) == 5) {
                if ((move_token[4] == 'n') && (decode_move_promotion(candidate) == PROM_KNIGHT))
                    return candidate;
                if ((move_token[4] == 'b') && (decode_move_promotion(candidate) == PROM_BISHOP))
                    return candidate;
                if ((move_token[4] == 'r') && (decode_move_promotion(candidate) == PROM_ROOK))
                    return candidate;
                if ((move_token[4] == 'q') && (decode_move_promotion(candidate) == PROM_QUEEN))
                    return candidate;
            }
        }
    }

    return INVALID_MOVE;
}

void command_go(EngineState *engine, char *go_options) {
    uint64_t start_time = get_time_ms();

    Depth depth = MAX_DEPTH;

    bool use_time_control = false;
    uint64_t time = 0;
    uint64_t inc = 0;
    int movestogo = 0;

    Color stm = engine->pos.side;

    char *token = strtok(go_options, " ");
    while (token != NULL) {
        if (strcmp(token, "depth") == 0) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                depth = (Depth)atoi(token);
            }
        } else if (strcmp(token, "wtime") == 0) {
            token = strtok(NULL, " ");
            if ((token != NULL) && (stm == WHITE)) {
                time = (uint64_t)atoi(token);
                use_time_control = true;
            }
        } else if (strcmp(token, "btime") == 0) {
            token = strtok(NULL, " ");
            if ((token != NULL) && (stm == BLACK)) {
                time = (uint64_t)atoi(token);
                use_time_control = true;
            }
        } else if (strcmp(token, "winc") == 0) {
            token = strtok(NULL, " ");
            if ((token != NULL) && (stm == WHITE)) {
                inc = (uint64_t)atoi(token);
                use_time_control = true;
            }
        } else if (strcmp(token, "binc") == 0) {
            token = strtok(NULL, " ");
            if ((token != NULL) && (stm == BLACK)) {
                inc = (uint64_t)atoi(token);
                use_time_control = true;
            }
        } else if (strcmp(token, "movestogo") == 0) {
            token = strtok(NULL, " ");
            if ((token != NULL)) {
                movestogo = atoi(token);
            }
        } else if (strcmp(token, "movetime") == 0) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                time = (uint64_t)atoi(token);
                movestogo = 1;
                use_time_control = true;
            }
        }
        token = strtok(NULL, " ");
    }

    printf(">>> Found limits: depth(%" PRIu8 ") time(%" PRIu64 ") inc(%" PRIu64 ") movestogo(%i)\n", depth, time, inc, movestogo);
    compute_time_to_search(&engine->search_ctx.tm, start_time, time, inc, movestogo, use_time_control);
    start_search(engine, depth);
}

void command_uci() {
    printf("id name Qurio\n");
    printf("id author Qurio\n");
    printf("option name Hash type spin default 8 min 1 max %i\n", MAX_HASH);
    printf("option name Threads type spin default 1 min 1 max %i\n", MAX_THREADS);
    printf("uciok\n");
    fflush(stdout);
}

void command_isready() {
    printf("readyok\n");
    fflush(stdout);
}

void command_ucinewgame(EngineState *engine) {
    command_stop(engine);
    clear_engine(engine);
}

void command_stop(EngineState *engine) {
    stop_search(engine);
}

void command_perft(EngineState *engine, char *perft_options) {
    int depth = 1;

    if (strncmp(perft_options, "depth", 5) == 0) {
        depth = atoi(perft_options + 6);
    }

    printf(">>> Running perft at depth %i\n", depth);
    divide_perft(engine->pos, depth);
}

void command_pprint(EngineState *engine) {
    print_position(engine->pos);
}

void command_setoption(EngineState *engine, char *setoption_options) {
    char *name_ptr = strstr(setoption_options, "name ");
    char *value_ptr = strstr(setoption_options, "value ");

    if (!name_ptr || !value_ptr)
        return;

    name_ptr += 5;
    value_ptr += 6;

    if (strncmp(name_ptr, "Hash", 4) == 0) {
        int val = atoi(value_ptr);
        if (val < 1)
            val = 1;
        if (val > MAX_HASH)
            val = MAX_HASH;
        resize_tt(&engine->table, (size_t)val);
    } else if (strncmp(name_ptr, "Threads", 7) == 0) {
        // Multi threading not available: do nothing
    }
}

void main_loop() {
    char line[8192];

    EngineState engine;
    init_tt(&engine.table, 8);
    clear_engine(&engine);

    while (fgets(line, sizeof(line), stdin) != NULL) {
        line[strcspn(line, "\r\n")] = '\0';

        if (strncmp(line, "quit", 4) == 0)
            return;

        if (strncmp(line, "position", 8) == 0) {
            command_position(&engine, line + 9);
            continue;
        }

        if (strncmp(line, "ucinewgame", 10) == 0) {
            command_ucinewgame(&engine);
            continue;
        }

        if (strncmp(line, "uci", 3) == 0) {
            command_uci();
            continue;
        }

        if (strncmp(line, "isready", 7) == 0) {
            command_isready();
            continue;
        }

        if (strncmp(line, "go", 2) == 0) {
            command_go(&engine, line + 3);
            continue;
        }

        if (strncmp(line, "stop", 4) == 0) {
            command_stop(&engine);
            continue;
        }

        if (strncmp(line, "perft", 5) == 0) {
            command_stop(&engine);
            command_perft(&engine, line + 6);
            continue;
        }

        if (strncmp(line, "bench", 5) == 0) {
            command_stop(&engine);
            bench();
            continue;
        }

        if (strncmp(line, "pprint", 6) == 0) {
            command_pprint(&engine);
            continue;
        }

        if (strncmp(line, "setoption", 9) == 0) {
            command_stop(&engine);
            command_setoption(&engine, line + 10);
            continue;
        }
    }
}

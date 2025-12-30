#include "uci.h"


void command_position(Position *pos, char* position_options) {
    if (strncmp(position_options, "fen", 3) == 0)
    {
        parse_fen(pos, position_options + 4);
    }
    else
    {
        set_start_position(pos);
    }

    char *moves = strstr(position_options, "moves");
    if (moves == NULL) return;
    moves += 6;

    char *move_token = strtok(moves, " ");
    while (move_token != NULL) {
        Move move = parse_move(*pos, move_token);
        make_move(pos, move);
        move_token = strtok(NULL, " ");
    }
}

Move parse_move(Position pos, char* move_token) {
    MoveList move_list = { .count = 0 };
    generate_pseudo_legals(pos, &move_list);

    int from_file = move_token[0] - 'a';
    int from_rank = (move_token[1] - '1');
    int to_file = move_token[2] - 'a';
    int to_rank = (move_token[3] - '1');

    Square from = (Square)((from_rank * NFILES) + from_file);
    Square to = (Square)((to_rank * NFILES) + to_file);

    for (int i = 0; i < move_list.count; i++)
    {
        Move candidate = move_list.moves[i];
        if ((from == decode_move_from(candidate)) && (to == decode_move_to(candidate)))
        {
            if (decode_move_type(candidate) != MOVE_PROM) return candidate;

            if (strlen(move_token) == 5)
            {
                if ((move_token[4] == 'k') && (decode_move_promotion(candidate) == PROM_KNIGHT)) return candidate;
                if ((move_token[4] == 'b') && (decode_move_promotion(candidate) == PROM_BISHOP)) return candidate;
                if ((move_token[4] == 'r') && (decode_move_promotion(candidate) == PROM_ROOK)) return candidate;
                if ((move_token[4] == 'q') && (decode_move_promotion(candidate) == PROM_QUEEN)) return candidate;
            }
        }
        
    }

    return (Move)0;
}

void command_go(Position pos, SearchContext *search, char* go_options) {
    int depth = 1;

    char *token = strtok(go_options, " ");
    while (token != NULL) {
        if (strcmp(token, "depth") == 0)
        {
            token = strtok(NULL, " ");
            if (token != NULL) {
                depth = atoi(token);
            }
        }
    }

    start_search(pos, search, depth);
}

void command_uci() {
    printf("id name Qurio\n");
    printf("id author Qurio\n");
    printf("uciok\n");
    fflush(stdout);
}

void command_isready() {
    printf("readyok\n");
    fflush(stdout);
}

void command_ucinewgame(Position *pos) {
    set_start_position(pos);
}


void command_stop(SearchContext *search) {
    stop_search(search);
}


void main_loop() {
    char line[2048];

    Position pos;
    set_start_position(&pos);

    SearchContext search;

    while (fgets(line, sizeof(line), stdin) != NULL)
    {
        line[strcspn(line, "\r\n")] = '\0';

        if (strncmp(line, "quit", 4) == 0) return;

        if (strncmp(line, "position", 8) == 0) {
            command_position(&pos, line + 9);
            // print_position(pos);
            continue;
        }
        
        if (strncmp(line, "ucinewgame", 10) == 0) {
            command_ucinewgame(&pos);
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
            command_go(pos, &search, line + 3);
            continue;
        }

        if (strncmp(line, "stop", 4) == 0) {
            command_stop(&search);
            continue;
        }
    }
}
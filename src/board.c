#include "board.h"

void print_bitboard(U64 bb) {
    printf("Bitboard value: %" PRIu64 "\n", bb);
    printf("Bitboard representation:\n");

    for (int rank = NRANKS - 1; rank >= 0; rank--) {
        printf("%i ", rank + 1);
        for (int file = 0; file < NFILES; file++) {
            int bit = (int)(1ULL & (bb >> (NFILES * rank + file)));
            if (bit) {
                printf("\033[0;32m1 \033[0m");
            } else {
                printf("\033[0;31m0 \033[0m");
            }
        }
        printf("\n");
    }
    printf("  a b c d e f g h\n");
}

void print_position_bitboard(Position pos) {
    printf("White Piece:\n");
    for (PieceType piece_type = 0; piece_type < NPIECES; piece_type++) {
        printf("Piece: %s \n", PIECE_NAMES[piece_type]);
        print_bitboard(pos.pieces[WHITE][piece_type]);
    }
    printf("Black Piece:\n");
    for (PieceType piece_type = 0; piece_type < NPIECES; piece_type++) {
        printf("Piece: %s \n", PIECE_NAMES[piece_type]);
        print_bitboard(pos.pieces[BLACK][piece_type]);
    }
}

U64 bb_square_value(U64 bb, Square sq) {
    return (1ULL & (bb >> sq));
}

Piece get_piece_on(Position pos, Square sq) {
    for (Color side = 0; side < 2; side++) {
        for (PieceType piece_type = 0; piece_type < NPIECES; piece_type++) {
            if (bb_square_value(pos.pieces[side][piece_type], sq) != 0) {
                return get_piece(piece_type, side);
            }
        }
    }
    return NOPIECE;
}

void print_position(Position pos) {
    printf("Position: \n");
    for (int rank = NRANKS - 1; rank >= 0; rank--) {
        printf("%i ", rank + 1);
        for (int file = 0; file < NFILES; file++) {
            Square sq = (Square)(NFILES * rank + file);
            Piece p = get_piece_on(pos, sq);

            switch (p) {
            case WPAWN:
                printf("%c ", 'P');
                break;
            case WKNIGHT:
                printf("%c ", 'N');
                break;
            case WBISHOP:
                printf("%c ", 'B');
                break;
            case WROOK:
                printf("%c ", 'R');
                break;
            case WQUEEN:
                printf("%c ", 'Q');
                break;
            case WKING:
                printf("%c ", 'K');
                break;
            case BPAWN:
                printf("%c ", 'p');
                break;
            case BKNIGHT:
                printf("%c ", 'n');
                break;
            case BBISHOP:
                printf("%c ", 'b');
                break;
            case BROOK:
                printf("%c ", 'r');
                break;
            case BQUEEN:
                printf("%c ", 'q');
                break;
            case BKING:
                printf("%c ", 'k');
                break;

            default:
                printf(". ");
            }
        }
        printf("\n");
    }
    printf("  a b c d e f g h\n");
    printf("Side to move: %s\n", (pos.side == WHITE) ? "W" : "B");
    printf("Castling right: %i\n", pos.castling);

    if (pos.enpassant != NOSQUARE) {
        printf("En passant square: %s\n", SQUARE_NAMES[pos.enpassant]);
    } else {
        printf("En passant square: NONE\n");
    }
    printf("Half move clock: %i\n", pos.halfmove_clock);
    printf("Hash: %" PRIu64 "\n", pos.hash);
}

U64 compute_hash(Position pos) {
    U64 hash = (pos.side == WHITE) ? 0 : get_side_zkey();

    for (Color side = 0; side < 2; side++) {
        for (PieceType piece_type = 0; piece_type < NPIECES; piece_type++) {
            U64 bb = pos.pieces[side][piece_type];
            while (bb) {
                Square sq = pop_lsb(&bb);
                hash ^= get_piece_square_zkey(get_piece(piece_type, side), sq);
            }
        }
    }

    hash ^= get_castling_zkey(pos.castling);

    if (pos.enpassant != NOSQUARE) {
        int file = pos.enpassant % NFILES;
        hash ^= get_enpassant_zkey(file);
    }

    return hash;
}

void set_start_position(Position *pos) {
    // Pawns
    pos->pieces[WHITE][PAWN] = 0xff00ULL;
    pos->pieces[BLACK][PAWN] = 0xff000000000000ULL;

    // Knights
    pos->pieces[WHITE][KNIGHT] = 0x42ULL;
    pos->pieces[BLACK][KNIGHT] = 0x4200000000000000LL;

    // Bishops
    pos->pieces[WHITE][BISHOP] = 0x24ULL;
    pos->pieces[BLACK][BISHOP] = 0x2400000000000000ULL;

    // Rooks
    pos->pieces[WHITE][ROOK] = 0x81LL;
    pos->pieces[BLACK][ROOK] = 0x8100000000000000ULL;

    // Queens
    pos->pieces[WHITE][QUEEN] = 0x8ULL;
    pos->pieces[BLACK][QUEEN] = 0x800000000000000ULL;

    // Kings
    pos->pieces[WHITE][KING] = 0x10ULL;
    pos->pieces[BLACK][KING] = 0x1000000000000000ULL;

    // Occupancies
    pos->occ[WHITE] = 0xffffULL;
    pos->occ[BLACK] = 0xffff000000000000ULL;

    pos->side = WHITE;
    pos->castling = 0b1111;
    pos->enpassant = NOSQUARE;
    pos->halfmove_clock = 0;

    pos->hash = compute_hash(*pos);
}

void parse_fen(Position *pos, char *fen) {
    // Pawns
    pos->pieces[WHITE][PAWN] = 0x0ULL;
    pos->pieces[BLACK][PAWN] = 0x0ULL;

    // Knights
    pos->pieces[WHITE][KNIGHT] = 0x0ULL;
    pos->pieces[BLACK][KNIGHT] = 0x0ULL;

    // Bishops
    pos->pieces[WHITE][BISHOP] = 0x0ULL;
    pos->pieces[BLACK][BISHOP] = 0x0ULL;

    // Rooks
    pos->pieces[WHITE][ROOK] = 0x0ULL;
    pos->pieces[BLACK][ROOK] = 0x0ULL;

    // Queens
    pos->pieces[WHITE][QUEEN] = 0x0ULL;
    pos->pieces[BLACK][QUEEN] = 0x0ULL;

    // Kings
    pos->pieces[WHITE][KING] = 0x0ULL;
    pos->pieces[BLACK][KING] = 0x0ULL;

    // Occupancies
    pos->occ[WHITE] = 0x0ULL;
    pos->occ[BLACK] = 0x0ULL;

    pos->castling = 0b0000;

    char *p = fen;

    // Piece Placement
    int rank = 7;
    int file = 0;
    while ((*p != '\0') && (*p != ' ')) {
        char c = *p;
        if (c == '/') {
            rank--;
            file = 0;
        } else if (c >= '0' && c <= '9') {
            int skip = c - '0';
            file += skip;
        } else {
            Square sq = (Square)((rank * NFILES) + file);
            switch (c) {
            case 'P':
                pos->pieces[WHITE][PAWN] = pos->pieces[WHITE][PAWN] | (1ULL << sq);
                break;
            case 'N':
                pos->pieces[WHITE][KNIGHT] = pos->pieces[WHITE][KNIGHT] | (1ULL << sq);
                break;
            case 'B':
                pos->pieces[WHITE][BISHOP] = pos->pieces[WHITE][BISHOP] | (1ULL << sq);
                break;
            case 'R':
                pos->pieces[WHITE][ROOK] = pos->pieces[WHITE][ROOK] | (1ULL << sq);
                break;
            case 'Q':
                pos->pieces[WHITE][QUEEN] = pos->pieces[WHITE][QUEEN] | (1ULL << sq);
                break;
            case 'K':
                pos->pieces[WHITE][KING] = pos->pieces[WHITE][KING] | (1ULL << sq);
                break;

            case 'p':
                pos->pieces[BLACK][PAWN] = pos->pieces[BLACK][PAWN] | (1ULL << sq);
                break;
            case 'n':
                pos->pieces[BLACK][KNIGHT] = pos->pieces[BLACK][KNIGHT] | (1ULL << sq);
                break;
            case 'b':
                pos->pieces[BLACK][BISHOP] = pos->pieces[BLACK][BISHOP] | (1ULL << sq);
                break;
            case 'r':
                pos->pieces[BLACK][ROOK] = pos->pieces[BLACK][ROOK] | (1ULL << sq);
                break;
            case 'q':
                pos->pieces[BLACK][QUEEN] = pos->pieces[BLACK][QUEEN] | (1ULL << sq);
                break;
            case 'k':
                pos->pieces[BLACK][KING] = pos->pieces[BLACK][KING] | (1ULL << sq);
                break;

            default:
                break;
            }
            file++;
        }
        p++;
    }

    // Side to move
    p++;
    char stm = *p;
    switch (stm) {
    case 'w':
        pos->side = WHITE;
        break;
    case 'b':
        pos->side = BLACK;
        break;

    default:
        break;
    }

    // Castling ability
    p++;
    p++;
    while ((*p != '\0') && (*p != ' ')) {
        char c = *p;
        switch (c) {
        case '-':
            pos->castling = 0b0000;
            break;
        case 'K':
            pos->castling = pos->castling | 0b0001;
            break;
        case 'Q':
            pos->castling = pos->castling | 0b0010;
            break;
        case 'k':
            pos->castling = pos->castling | 0b0100;
            break;
        case 'q':
            pos->castling = pos->castling | 0b1000;
            break;

        default:
            break;
        }
        p++;
    }

    // En passant target square
    p++;
    if (*p == '-') {
        pos->enpassant = NOSQUARE;
    } else {
        file = (*p++) - 'a';
        rank = ((*p++) - '1');
        pos->enpassant = (Square)((rank * NFILES) + file);
    }

    // Half move clock
    p++;
    pos->halfmove_clock = atoi(p);

    // Compute occupancies
    for (PieceType piece_type = 0; piece_type < NPIECES; piece_type++) {
        pos->occ[WHITE] = pos->occ[WHITE] | pos->pieces[WHITE][piece_type];
        pos->occ[BLACK] = pos->occ[BLACK] | pos->pieces[BLACK][piece_type];
    }

    pos->hash = compute_hash(*pos);
}

void make_move(Position *pos, Move move) {
    Square from = decode_move_from(move);
    Square to = decode_move_to(move);
    PromotionType prom = decode_move_promotion(move);
    MoveType type = decode_move_type(move);

    Color stm = pos->side;
    Color op = stm ^ 1;

    bool is_capture = (pos->occ[op] & (1ULL << to));
    Piece moving_piece = get_piece_on(*pos, from);
    Piece target_piece = (is_capture) ? get_piece_on(*pos, to) : NOPIECE;

    PieceType moving_piece_type = get_piece_type(moving_piece);
    PieceType target_piece_type = get_piece_type(target_piece);

    switch (type) {
    case MOVE_DEFAULT:
        pos->pieces[stm][moving_piece_type] = (pos->pieces[stm][moving_piece_type] ^ (1ULL << from)) | (1ULL << to);
        pos->occ[stm] = (pos->occ[stm] ^ (1ULL << from)) | (1ULL << to);
        pos->hash ^= get_piece_square_zkey(moving_piece, from) ^ get_piece_square_zkey(moving_piece, to);

        if (is_capture) {
            pos->pieces[op][target_piece_type] = (pos->pieces[op][target_piece_type] ^ (1ULL << to));
            pos->occ[op] = (pos->occ[op] ^ (1ULL << to));
            pos->hash ^= get_piece_square_zkey(target_piece, to);
        }

        break;
    case MOVE_PROM:
        pos->pieces[stm][moving_piece_type] = (pos->pieces[stm][moving_piece_type] ^ (1ULL << from));
        pos->pieces[stm][(PieceType)(prom)] = pos->pieces[stm][(PieceType)(prom)] | (1ULL << to);
        pos->occ[stm] = (pos->occ[stm] ^ (1ULL << from)) | (1ULL << to);
        pos->hash ^= get_piece_square_zkey(moving_piece, from) ^ get_piece_square_zkey(get_piece((PieceType)(prom), stm), to);

        if (is_capture) {
            pos->pieces[op][target_piece_type] = (pos->pieces[op][target_piece_type] ^ (1ULL << to));
            pos->occ[op] = (pos->occ[op] ^ (1ULL << to));
            pos->hash ^= get_piece_square_zkey(target_piece, to);
        }

        break;
    case MOVE_ENPASSANT:
        pos->pieces[stm][moving_piece_type] = (pos->pieces[stm][moving_piece_type] ^ (1ULL << from)) | (1ULL << to);
        pos->occ[stm] = (pos->occ[stm] ^ (1ULL << from)) | (1ULL << to);
        pos->hash ^= get_piece_square_zkey(moving_piece, from) ^ get_piece_square_zkey(moving_piece, to);

        Square enpassant_target = (stm == WHITE) ? (to - 8) : (to + 8);
        pos->pieces[op][PAWN] = (pos->pieces[op][PAWN] ^ (1ULL << enpassant_target));
        pos->occ[op] = (pos->occ[op] ^ (1ULL << enpassant_target));
        pos->hash ^= get_piece_square_zkey(get_piece(PAWN, op), enpassant_target);

        break;
    case MOVE_CASTLING:
        pos->pieces[stm][KING] = (pos->pieces[stm][KING] ^ (1ULL << from)) | (1ULL << to);
        pos->occ[stm] = (pos->occ[stm] ^ (1ULL << from)) | (1ULL << to);
        pos->hash ^= get_piece_square_zkey(get_piece(KING, stm), from) ^ get_piece_square_zkey(get_piece(KING, stm), to);

        Square starting_rook;
        Square ending_rook;
        if (to == G1) {
            starting_rook = H1;
            ending_rook = F1;
        } else if (to == C1) {
            starting_rook = A1;
            ending_rook = D1;
        } else if (to == G8) {
            starting_rook = H8;
            ending_rook = F8;
        } else {
            starting_rook = A8;
            ending_rook = D8;
        }

        pos->pieces[stm][ROOK] = (pos->pieces[stm][ROOK] ^ (1ULL << starting_rook)) | (1ULL << ending_rook);
        pos->occ[stm] = (pos->occ[stm] ^ (1ULL << starting_rook)) | (1ULL << ending_rook);
        pos->hash ^= get_piece_square_zkey(get_piece(ROOK, stm), starting_rook) ^ get_piece_square_zkey(get_piece(ROOK, stm), ending_rook);

        break;

    default:
        break;
    }

    if (pos->castling) {
        Square king_square = (stm == WHITE) ? E1 : E8;
        pos->hash ^= get_castling_zkey(pos->castling);

        if (from == king_square) {
            CastlingRight mask = (stm == WHITE) ? (WCASTLING_KING | WCASTLING_QUEEN) : (BCASTLING_KING | BCASTLING_QUEEN);
            pos->castling = pos->castling & ~(mask);
        }

        if (from == A1 || to == A1) {
            CastlingRight mask = WCASTLING_QUEEN;
            pos->castling = pos->castling & ~(mask);
        }

        if (from == H1 || to == H1) {
            CastlingRight mask = WCASTLING_KING;
            pos->castling = pos->castling & ~(mask);
        }

        if (from == A8 || to == A8) {
            CastlingRight mask = BCASTLING_QUEEN;
            pos->castling = pos->castling & ~(mask);
        }

        if (from == H8 || to == H8) {
            CastlingRight mask = BCASTLING_KING;
            pos->castling = pos->castling & ~(mask);
        }
        pos->hash ^= get_castling_zkey(pos->castling);
    }

    if (pos->enpassant != NOSQUARE) {
        pos->hash ^= get_enpassant_zkey(pos->enpassant % NFILES);
    }

    int distance = (stm == WHITE) ? (to - from) : (from - to);
    if ((moving_piece_type == PAWN) && (distance == 16)) {
        pos->enpassant = (stm == WHITE) ? (from + 8) : (from - 8);
        pos->hash ^= get_enpassant_zkey(pos->enpassant % NFILES);
    } else {
        pos->enpassant = NOSQUARE;
    }

    if (is_capture || moving_piece_type == PAWN) {
        pos->halfmove_clock = 0;
    } else {
        pos->halfmove_clock++;
    }

    pos->side = op;
    pos->hash ^= get_side_zkey();
}
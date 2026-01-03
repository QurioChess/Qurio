#include "zobrist.h"

/*  Written in 2015 by Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide.

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR
IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. */

/* This is a fixed-increment version of Java 8's SplittableRandom generator
   See http://dx.doi.org/10.1145/2714064.2660195 and
   http://docs.oracle.com/javase/8/docs/api/java/util/SplittableRandom.html

   It is a very fast generator passing BigCrush, and it can be useful if
   for some reason you absolutely want 64 bits of state. */

// SplitMix64 seed from "qurio" for deterministic Zobrist keys
// hex(int.from_bytes(hashlib.sha256(b"qurio").digest()[:8], 'little'))
static U64 x = 0x400a1296699de972ULL;

U64 next() {
    U64 z = (x += 0x9e3779b97f4a7c15);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
    z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
    return z ^ (z >> 31);
}

U64 PIECE_SQUARE_ZKEYS[12][64];
U64 CASTLING_ZKEYS[16];
U64 ENPASSANT_ZKEYS[8];
U64 SIDE_ZKEY;

void init_zobrist() {
    for (PieceType piece_type = 0; piece_type < NPIECES; piece_type++) {
        for (Square sq = 0; sq < NSQUARES; sq++) {
            PIECE_SQUARE_ZKEYS[get_piece(piece_type, WHITE)][sq] = next();
            PIECE_SQUARE_ZKEYS[get_piece(piece_type, BLACK)][sq] = next();
        }
    }

    for (CastlingRight i = 0; i < 16; ++i)
        CASTLING_ZKEYS[i] = next();
    for (int file = 0; file < 8; ++file)
        ENPASSANT_ZKEYS[file] = next();
    SIDE_ZKEY = next();
}

#include <stdio.h>

#include "bench.h"
#include "negamax.h"
#include "uci.h"
#include "zobrist.h"

int main(int argc, char *argv[]) {
    printf("Qurio - Chess engine\n");
    init_zobrist();
    init_negamax_tables();

    if (argc > 1 && strcmp(argv[1], "bench") == 0) {
        bench();
        return 0;
    }

    main_loop();
    return 0;
}

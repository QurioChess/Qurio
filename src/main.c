#include <stdio.h>

#include "bench.h"
#include "uci.h"

int main(int argc, char *argv[]) {
    printf("Qurio - Chess engine\n");

    if (argc > 1 && strcmp(argv[1], "bench") == 0) {
        bench();
        return 0;
    }

    main_loop();
    return 0;
}

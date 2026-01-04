#include "bench.h"

void bench() {
    EngineState engine = {0};
    init_tt(&engine.table, 8);

    Depth depth = BENCH_DEPTH;
    bool use_time_control = false;
    uint64_t time = 0;
    uint64_t inc = 0;
    int movestogo = 0;

    uint64_t start = get_time_ms();
    U64 nodes = 0ULL;
    for (int i = 0; i < NFENS; i++) {
        reset_history(&engine.history);
        parse_fen(&engine.pos, BENCH_FENS[i]);
        push_hash(&engine.history, engine.pos.hash);

        uint64_t start_time = get_time_ms();
        compute_time_to_search(&engine.search_ctx.tm, start_time, time, inc, movestogo, use_time_control);

        start_search(&engine, depth);
        wait_search(&engine);

        nodes += engine.thread_ctxs[0].nodes;
    }

    uint64_t end = get_time_ms();
    uint64_t time_taken_ms = end - start;
    time_taken_ms = (time_taken_ms > 0) ? time_taken_ms : 1;
    double time_taken_s = (double)time_taken_ms / 1000.0;

    // OpenBench-friendly output
    printf("\n%" PRIu64 " nodes\n", nodes);
    printf("%" PRIu64 " nps\n", (uint64_t)((double)nodes / time_taken_s));

    // Human-friendly output
    printf("\n=== Benchmark Summary ===\n");
    printf("Total nodes searched: %" PRIu64 "\n", nodes);
    printf("Time taken          : %" PRIu64 " ms (%.3f s)\n", time_taken_ms, time_taken_s);
    printf("Nodes per second    : %" PRIu64 "\n", (uint64_t)((double)nodes / time_taken_s));
}

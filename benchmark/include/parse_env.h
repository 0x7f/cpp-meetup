#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void parse_env(const char** host, uint16_t* port, uint16_t* num_threads,
                      bool* skip_validation) {
    *host = "0.0.0.0";

    const char* port_str = getenv("BENCHMARK_PORT");
    if (!port_str) {
        fprintf(stderr, "BENCHMARK_PORT not configured.\n");
        exit(1);
    }
    *port = static_cast<uint16_t>(atoi(port_str));

    const char* threads_str = getenv("BENCHMARK_THREADS");
    if (!threads_str) {
        fprintf(stderr, "BENCHMARK_THREADS not configured.\n");
        exit(1);
    }

    *num_threads = static_cast<uint16_t>(atoi(threads_str));
    if (*num_threads < 0) {
        fprintf(stderr, "BENCHMARK_THREADS must be >=0.\n");
        exit(1);
    }

    const char* skip_validation_str = getenv("BENCHMARK_SKIP_VALIDATION");
    if (skip_validation_str && strcmp("1", skip_validation_str) == 0) {
        *skip_validation = true;
    }

    printf("Configuration: BENCHMARK_PORT=%d BENCHMARK_THREADS=%d "
           "BENCHMARK_SKIP_VALIDATION=%d\n",
           *port, *num_threads, *skip_validation);
}

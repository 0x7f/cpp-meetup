#!/bin/bash -eu

trap ctrl_c INT
function ctrl_c() {
    exit 1
}

RUNS=3
THREADS_MIN=1
THREADS_MAX=3
BENCHARKS="empty-evhtp
  empty-httpp
  gason-evhtp
  gason-httpp
  jsoncpp-evhtp
  jsoncpp-httpp
  pbjson-evhtp
  pbjson-httpp
  protog-evhtp
  protog-httpp
  rapidjson-evhtp
  rapidjson-httpp"

mkdir -p results

function runBenchmark() {
    BENCHMARK_PORT=8080 BENCHMARK_THREADS=$2 BENCHMARK_SKIP_VALIDATION=1 \
        ./build/benchmark/${1}d >/dev/null &
    BPID=$!
    sleep 1
    wrk -s share/post.lua -c 100 -d 120s -t 3 http://localhost:8080/add \
        1> "results/result_${1}_t${2}_r${3}_out.txt" \
        2> "results/result_${1}_t${2}_r${3}_err.txt"
    (kill -TERM $BPID || kill -KILL $BPID || true) > /dev/null
    while (ps -p $BPID > /dev/null); do
        echo "waiting for $BPID to shut down"
        sleep 1
    done
}

for B in $BENCHARKS; do
    for T in `seq $THREADS_MIN $THREADS_MAX`; do
        for R in `seq $RUNS`; do
            echo "$B $T $R"
            runBenchmark $B $T $R
        done
    done
done

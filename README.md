# protog benchmarks

See https://github.com/0x7f/protog

## building

Install build-essentials, git, protobuf 2.6.1 and required dependencies of [daedric/httpp](https://github.com/daedric/httpp/).

```
git submodule update --init --recursive
(cd thirdparty/httpp/third_party/commonpp/ && mkdir -p build && cd build && cmake .. && make)
(cd thirdparty/httpp/ && mkdir -p build && cd build && cmake .. && make)
(cd thirdparty/protog/ && mkdir -p build && cd build && cmake .. && make)
(cd thirdparty/jsoncpp/ && mkdir -p build && cd build && cmake .. && make)
(cd thirdparty/rapidjson/ && mkdir -p build && cd build && cmake .. && make)
```

```
(mkdir -p build && cd build && cmake .. && make)
```

## benchmark

The following tools were executed for the benchmark:

```
export BENCHMARK_PORT=8080
export BENCHMARK_THREADS=4
# first, all the commands without request validation
export BENCHMARK_SKIP_VALIDATION=1
./build/benchmark/empty/emptyd
./build/benchmark/gason/gasond
./build/benchmark/jsoncpp/jsoncppd
./build/benchmark/rapidjson/rapidjsond
nodejs benchmark/nodejs/server.js
# then, all tools doing schema validation
export BENCHMARK_SKIP_VALIDATION=0
./build/benchmark/protog/protogd
./build/benchmark/pbjson/pbjsond
./build/benchmark/rapidjson/rapidjsond
nodejs benchmark/nodejs/server.js
```

and the following command was used to benchmark the tools:

```
wrk -s share/post.lua -c 200 -d 60s -t 12 http://localhost:8080/add
```

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

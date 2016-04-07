# protog benchmarks

See https://github.com/0x7f/protog

## building

Install required dependencies of [daedric/httpp](https://github.com/daedric/httpp/).

```
git submodule update --init --recursive
mkdir -p thirdparty/httpp/third_party/commonpp/build
(cd thirdparty/httpp/third_party/commonpp/build && cmake .. && make)
mkdir -p thirdparty/httpp/build
(cd thirdparty/httpp/build && cmake .. && make)
mkdir -p thirdparty/protog/build
(cd thirdparty/protog/build && cmake .. && make)
mkdir -p thirdparty/rapidjson/build
(cd thirdparty/rapidjson/build && cmake .. && make)
```

```
mkdir build && cd build
cmake .. && make
```

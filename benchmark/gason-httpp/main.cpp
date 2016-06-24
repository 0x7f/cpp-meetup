#include <algorithm>
#include <chrono>
#include <fstream>
#include <thread>

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>

#include <httpp/HttpServer.hpp>
#include <httpp/http/Utils.hpp>
#include <httpp/utils/Exception.hpp>

#include <commonpp/thread/ThreadPool.hpp>

#include <gason.h>

using HTTPP::HTTP::Connection;
using HTTPP::HTTP::helper::ReadWholeRequest;
using HTTPP::HTTP::setShouldConnectionBeClosed;
using HTTPP::UTILS::convert_boost_ec_to_std_ec;

static bool skip_validation = false;

void connection_handler(Connection* connection) {
    read_whole_request(connection, [](std::unique_ptr<ReadWholeRequest> handle,
                                      const boost::system::error_code& ec) {
        if (ec) {
            throw convert_boost_ec_to_std_ec(ec);
        }

        auto& body = handle->body;
        body.push_back('\0');
        auto& connection = handle->connection;
        auto& response = connection->response();

        char *endptr;
        JsonValue value;
        JsonAllocator allocator;
        int status = jsonParse(body.data(), &endptr, &value, allocator);
        if (status == JSON_OK) {
            response.setCode(HTTPP::HTTP::HttpCode::Ok)
                    .setBody("OK:"); // TODO: how to retrieve id?
        } else {
            response.setCode(HTTPP::HTTP::HttpCode::BadRequest)
                    .setBody("ERROR:" + std::string{jsonStrError(status)});
        }

        setShouldConnectionBeClosed(connection->request(), connection->response());
        connection->sendResponse(); // connection pointer may become invalid
    });
}

int main() {
    boost::log::core::get()->set_filter(boost::log::trivial::severity >=
                                        boost::log::trivial::warning);

    const char* host = "localhost";
    const char* port = getenv("BENCHMARK_PORT");
    if (!port) {
        fprintf(stderr, "BENCHMARK_PORT not configured.\n");
        return 1;
    }

    const char* threads = getenv("BENCHMARK_THREADS");
    if (!threads) {
        fprintf(stderr, "BENCHMARK_THREADS not configured.\n");
        return 1;
    }

    const char* skip_validation_str = getenv("BENCHMARK_SKIP_VALIDATION");
    if (skip_validation_str && strcmp("1", skip_validation_str) == 0) {
        skip_validation = true;
    }

    int numThreads = atoi(threads);
    assert(numThreads >= 0);

    printf("Configuration: BENCHMARK_PORT=%s BENCHMARK_THREADS=%d BENCHMARK_SKIP_VALIDATION=%d\n",
            port, numThreads, skip_validation);

    commonpp::thread::ThreadPool threadPool{static_cast<size_t>(numThreads)};

    HTTPP::HttpServer server{threadPool};
    server.start();
    server.setSink(std::bind(&connection_handler, std::placeholders::_1));
    server.bind(host, port);

    printf("READY\n");

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

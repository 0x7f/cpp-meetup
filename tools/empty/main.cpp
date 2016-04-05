#include <algorithm>
#include <chrono>
#include <thread>

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>

#include <httpp/HttpServer.hpp>
#include <httpp/http/Utils.hpp>

#include <commonpp/thread/ThreadPool.hpp>

void body_handler(const HTTPP::HTTP::Request& request, HTTPP::HTTP::Connection* connection,
                  const boost::system::error_code& ec,
                  const char* buffer, size_t n) {
    // TODO: why is no eof provided?
    if (ec == boost::asio::error::eof || n == 0) {
        connection->response()
            .setCode(HTTPP::HTTP::HttpCode::Ok)
            .setBody("OK");
        HTTPP::HTTP::setShouldConnectionBeClosed(request, connection->response());
        connection->sendResponse(); // connection pointer may become invalid
    } else if (ec) {
        throw std::runtime_error("MOOOOOOOOOO");
    } else {
        // ignore body
    }
}

void connection_handler(HTTPP::HTTP::Connection* connection) {
    auto& request = connection->request();
    auto it = std::find_if(
        request.headers.begin(), request.headers.end(), [](const HTTPP::HTTP::HeaderRef& s) {
            return ::strncasecmp("Content-Length", s.first.data(), 14) == 0;
        });

    size_t size = 0;
    if (it != request.headers.end())
    {
        size = atoi(it->second.data());
    }

    if (size) {
        connection->read(size,
                std::bind(&body_handler,
                    request,
                    connection,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3));
    } else {
        connection->response()
            .setCode(HTTPP::HTTP::HttpCode::Ok)
            .setBody("OK");
        HTTPP::HTTP::setShouldConnectionBeClosed(request, connection->response());
        connection->sendResponse(); // connection pointer may become invalid
    }
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

    int numThreads = atoi(threads);
    assert(numThreads >= 0);
    commonpp::thread::ThreadPool threadPool{static_cast<size_t>(numThreads)};

    HTTPP::HttpServer server{threadPool};
    server.start();
    server.setSink(&connection_handler);
    server.bind(host, port);

    printf("READY\n");

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

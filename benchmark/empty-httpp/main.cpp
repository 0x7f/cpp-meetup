#include <algorithm>
#include <chrono>
#include <thread>

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>

#include <httpp/HttpServer.hpp>
#include <httpp/http/Utils.hpp>
#include <httpp/utils/Exception.hpp>

#include <commonpp/thread/ThreadPool.hpp>

#include "parse_env.h"

void connection_handler(HTTPP::HTTP::Connection* connection) {
    read_whole_request(connection, [](std::unique_ptr<HTTPP::HTTP::helper::ReadWholeRequest> handle,
                                      const boost::system::error_code& ec) {
        if (ec) {
            throw HTTPP::UTILS::convert_boost_ec_to_std_ec(ec);
        }

        auto connection = handle->connection;
        connection->response().setCode(HTTPP::HTTP::HttpCode::Ok).setBody("OK");
        HTTPP::HTTP::setShouldConnectionBeClosed(connection->request(),
                                                 connection->response());
        connection->sendResponse(); // connection pointer may become invalid
    });
}

int main() {
    boost::log::core::get()->set_filter(boost::log::trivial::severity >=
                                        boost::log::trivial::warning);

    const char* host;
    uint16_t port, num_threads;
    bool skip_validation;
    parse_env(&host, &port, &num_threads, &skip_validation);

    commonpp::thread::ThreadPool threadPool{static_cast<size_t>(num_threads)};

    HTTPP::HttpServer server{threadPool};
    server.start();
    server.setSink(&connection_handler);
    server.bind(host, std::to_string(port));

    printf("READY\n");

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

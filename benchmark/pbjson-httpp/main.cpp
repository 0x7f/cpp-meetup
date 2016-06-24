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

#include <rapidjson/document.h>

#include <pbjson.hpp>

#include "bidrequest_parser.pb.h"

using namespace ::com::google::openrtb;

static bool parseRequest(const std::vector<char>& body, BidRequest& msg, std::string& err) {
    rapidjson::Document d;
    d.Parse(body.data());
    if (d.HasParseError()) {
         err += d.GetParseError();
         return false;
    }

    int rc = ::pbjson::jsonobject2pb(&d, &msg, err);
    if (rc < 0) {
        return false;
    }

    return true;
}

void connection_handler(HTTPP::HTTP::Connection* connection) {
    read_whole_request(connection, [](std::unique_ptr<HTTPP::HTTP::helper::ReadWholeRequest> handle,
                                      const boost::system::error_code& ec) {
        if (ec) {
            throw HTTPP::UTILS::convert_boost_ec_to_std_ec(ec);
        }

        handle->body.push_back('\0');
        auto& connection = handle->connection;
        auto& response = connection->response();

        std::string err;
        BidRequest bidRequest;
        if (parseRequest(handle->body, bidRequest, err)) {
            response.setCode(HTTPP::HTTP::HttpCode::Ok).setBody("OK:" + bidRequest.id());
        } else {
            response.setCode(HTTPP::HTTP::HttpCode::BadRequest).setBody("ERROR:" + err);
        }

        HTTPP::HTTP::setShouldConnectionBeClosed(connection->request(),
                                                 connection->response());
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

    int numThreads = atoi(threads);
    assert(numThreads >= 0);

    printf("Configuration: BENCHMARK_PORT=%s BENCHMARK_THREADS=%d BENCHMARK_SKIP_VALIDATION=%d\n",
            port, numThreads, 0);

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

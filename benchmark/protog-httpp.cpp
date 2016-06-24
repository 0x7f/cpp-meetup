#include <algorithm>
#include <chrono>
#include <thread>

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>

#include <httpp/HttpServer.hpp>
#include <httpp/http/Utils.hpp>

#include <commonpp/thread/ThreadPool.hpp>

#include "parse_env.h"
#include "bidrequest_parser.pb.h"

using namespace ::com::google::openrtb;

struct OpenRtbRequest {
    BidRequest bidRequest;
    bidrequest_parser_state_t state;
    HTTPP::HTTP::Connection* connection;
};

void body_handler(OpenRtbRequest* req,
                  const boost::system::error_code& ec,
                  const char* buffer, size_t n) {
    // TODO: why is no eof provided?
    if (ec == boost::asio::error::eof || n == 0) {
        int rc = bidrequest_parser_complete(req->state);
        if (rc != 0) {
            throw std::runtime_error("bidrequest_parser_complete failed :(");
        }
        req->connection->response()
            .setCode(HTTPP::HTTP::HttpCode::Ok)
            .setBody("OK:" + req->bidRequest.id());
        HTTPP::HTTP::setShouldConnectionBeClosed(req->connection->request(),
                                                 req->connection->response());
        req->connection->sendResponse(); // connection pointer may become invalid
        bidrequest_parser_free(req->state);
        delete req;
    } else if (ec) {
        delete req;
        throw std::runtime_error("MOOOOOOOOOOOOOOO");
    } else {
        // TODO: handle body std::cout.write(buffer,n) << std::endl;
        int rc = bidrequest_parser_on_chunk(req->state, const_cast<char*>(buffer), n);
        if (rc != 0) {
            throw std::runtime_error("bidrequest_parser_on_chunk failed :(");
        }
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
        OpenRtbRequest* req = new OpenRtbRequest();
        req->connection = connection;
        req->state = bidrequest_parser_init(req->bidRequest);
        connection->read(size,
                std::bind(&body_handler,
                    req,
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

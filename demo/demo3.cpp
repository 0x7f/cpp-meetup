#include <algorithm>
#include <chrono>
#include <memory>
#include <thread>

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>

#include <httpp/HttpServer.hpp>
#include <httpp/http/Utils.hpp>
#include <httpp/utils/Exception.hpp>

#include <commonpp/thread/ThreadPool.hpp>

#include "bidrequest_parser.pb.h"

using namespace ::com::google::openrtb;

struct OpenRtbRequest {
  BidRequest bidRequest;
  bidrequest_parser_state_t state;
};

void body_handler(std::shared_ptr<OpenRtbRequest> req, HTTPP::HTTP::Connection *connection,
                  const boost::system::error_code &ec, const char *buf,
                  size_t bufLen) {
  if (ec) {
    throw HTTPP::UTILS::convert_boost_ec_to_std_ec(ec);
  }

  if (bufLen > 0) {
    if (bidrequest_parser_on_chunk(req->state, const_cast<char *>(buf), bufLen) != 0) {
      throw std::runtime_error("bidrequest_parser_on_chunk failed :(");
    }
    return;
  }

  if (bidrequest_parser_complete(req->state) == 0) {
    connection->response()
        .setCode(HTTPP::HTTP::HttpCode::Ok)
        .setBody("OK:" + req->bidRequest.id());
  } else {
    connection->response()
        .setCode(HTTPP::HTTP::HttpCode::BadRequest)
        .setBody("ERROR:");
  }
  HTTPP::HTTP::setShouldConnectionBeClosed(connection->request(),
                                           connection->response());
  connection->sendResponse();
  bidrequest_parser_free(req->state);
}

void connection_handler(HTTPP::HTTP::Connection *connection) {
  auto &request = connection->request();
  const auto &headers = request.headers;
  const auto it = std::find_if(
      headers.begin(), headers.end(), [](const HTTPP::HTTP::HeaderRef &s) {
        return ::strncasecmp("Content-Length", s.first.data(), 14) == 0;
      });

  size_t size = 0;
  if (it != request.headers.end()) {
    size = atoi(it->second.data());
  }

  if (!size) {
    connection->response().setCode(HTTPP::HTTP::HttpCode::BadRequest);
    HTTPP::HTTP::setShouldConnectionBeClosed(request, connection->response());
    connection->sendResponse();
    return;
  }

  auto req = std::make_shared<OpenRtbRequest>();
  req->state = bidrequest_parser_init(req->bidRequest);
  connection->read(size, std::bind(&body_handler, req, connection,
                                   std::placeholders::_1, std::placeholders::_2,
                                   std::placeholders::_3));
}

int main() {
  HTTPP::HttpServer server;
  server.start();
  server.setSink(&connection_handler);
  server.bind("localhost", "8080");

  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

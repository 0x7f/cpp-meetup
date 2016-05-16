#include <httpp/HttpServer.hpp>
#include <httpp/http/Utils.hpp>
#include <httpp/utils/Exception.hpp>
#include <openrtb.pb.h>
#include <pbjson.hpp>

int main() {
  HTTPP::HttpServer server;
  server.start();
  server.setSink([](HTTPP::HTTP::Connection *connection) {
    HTTPP::HTTP::read_whole_request(
        connection,
        [](std::unique_ptr<HTTPP::HTTP::helper::ReadWholeRequest> handle,
           const boost::system::error_code &ec) {
          if (ec) {
            throw HTTPP::UTILS::convert_boost_ec_to_std_ec(ec);
          }

          auto &connection = handle->connection;
          handle->body.push_back('\0'); // meh.

          std::string err;
          com::google::openrtb::BidRequest bidRequest;
          if (pbjson::json2pb(handle->body.data(), &bidRequest, err) == 0 &&
              bidRequest.IsInitialized()) {
            connection->response()
                .setCode(HTTPP::HTTP::HttpCode::Ok)
                .setBody("OK: " + bidRequest.id());
          } else {
            connection->response()
                .setCode(HTTPP::HTTP::HttpCode::BadRequest)
                .setBody("ERROR: " + err);
          }

          HTTPP::HTTP::setShouldConnectionBeClosed(connection->request(),
                                                   connection->response());
          connection->sendResponse();
        });
  });
  server.bind("localhost", "8080");

  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

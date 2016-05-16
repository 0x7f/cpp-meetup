#include <httpp/HttpServer.hpp>
#include <httpp/http/Utils.hpp>
#include <httpp/utils/Exception.hpp>
#include <json/json.h>

int main() {
  HTTPP::HttpServer server;
  server.start();
  server.setSink([](HTTPP::HTTP::Connection *connection) {
    HTTPP::HTTP::read_whole_request(
        connection,
        [](std::unique_ptr<HTTPP::HTTP::helper::ReadWholeRequest> handle,
           const boost::system::error_code &ec) {
          auto &connection = handle->connection;
          if (ec) {
            throw HTTPP::UTILS::convert_boost_ec_to_std_ec(ec);
          }

          Json::Value root;
          const auto &begin = handle->body.data();
          const auto &end = begin + handle->body.size();
          if (Json::Reader().parse(begin, end, root)) {
            connection->response()
                .setCode(HTTPP::HTTP::HttpCode::Ok)
                .setBody("OK:" + root["id"].asString());
          } else {
            connection->response()
                .setCode(HTTPP::HTTP::HttpCode::BadRequest)
                .setBody("ERROR:");
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

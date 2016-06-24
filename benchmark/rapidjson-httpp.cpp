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
#include <rapidjson/filereadstream.h>
#include <rapidjson/reader.h>
#include <rapidjson/schema.h>
#include <rapidjson/stream.h>

#include "parse_env.h"

using namespace ::rapidjson;

static bool skip_validation = false;

static std::string getFileContent(const std::string& path) {
  std::ifstream file(path);
  return std::string{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

void connection_handler(SchemaValidator* sv, HTTPP::HTTP::Connection* connection) {
    read_whole_request(connection, [sv](std::unique_ptr<HTTPP::HTTP::helper::ReadWholeRequest> handle,
                                      const boost::system::error_code& ec) {
        if (ec) {
            throw HTTPP::UTILS::convert_boost_ec_to_std_ec(ec);
        }

        auto& body = handle->body;
        body.push_back('\0');
        auto& connection = handle->connection;
        auto& response = connection->response();

        if (!skip_validation) {
            Reader reader;
            StringStream ss(body.data());
            sv->Reset();
            if (!reader.Parse(ss, *sv) && reader.GetParseErrorCode() != kParseErrorTermination) {
                response.setCode(HTTPP::HTTP::HttpCode::BadRequest);
                response.connectionShouldBeClosed(true);
                connection->sendResponse(); // connection pointer may become invalid
                return;
            }

            if (!sv->IsValid()) {
                response.setCode(HTTPP::HTTP::HttpCode::BadRequest);
                response.connectionShouldBeClosed(true);
                connection->sendResponse(); // connection pointer may become invalid
                return;
            }
        }

        Document d;
        try {
            d.Parse(body.data(), body.size());
            if (d.HasMember("id")) {
                response.setCode(HTTPP::HTTP::HttpCode::Ok)
                    .setBody("OK:"+std::string{d["id"].GetString()});
            } else {
                response.setCode(HTTPP::HTTP::HttpCode::BadRequest);
            }
        } catch(...) {
            response.setCode(HTTPP::HTTP::HttpCode::InternalServerError);
        }
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
    parse_env(&host, &port, &num_threads, &skip_validation);

    commonpp::thread::ThreadPool threadPool{static_cast<size_t>(num_threads)};

    const auto path = "share/openrtb-schema_bid-request_v2-3.json";
    const auto content = getFileContent(path);
    Document d;
    d.Parse(content.c_str(), content.size());
    if (d.HasParseError()) {
        fprintf(stderr, "Schema file '%s' is not a valid JSON\n", path);
        return 1;
    }

    SchemaDocument sd(d);
    SchemaValidator sv(sd);

    HTTPP::HttpServer server{threadPool};
    server.start();
    server.setSink(std::bind(&connection_handler, &sv, std::placeholders::_1));
    server.bind(host, std::to_string(port));

    printf("READY\n");

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

#include <fstream>

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/reader.h>
#include <rapidjson/schema.h>
#include <rapidjson/stream.h>

#include "evhtp_util.h"
#include "parse_env.h"

using namespace ::rapidjson;

static bool skip_validation = false;

static std::string getFileContent(const std::string& path) {
  std::ifstream file(path);
  return std::string{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

static void add_cb(evhtp_request_t* req, void* a) {
    if (req->method != htp_method_POST) {
        evhtp_send_reply(req, EVHTP_RES_METHNALLOWED);
        return;
    }


    size_t len;
    auto buf = extract_body(req, &len);
    if (!buf) {
        evhtp_send_reply(req, EVHTP_RES_500);
        return;
    }

#if 1
    if (!skip_validation) {
        Reader reader;
        StringStream ss(buf);
        auto& sv = *static_cast<SchemaValidator*>(a);
        sv.Reset();
        if (!reader.Parse(ss, sv) &&
            reader.GetParseErrorCode() != kParseErrorTermination) {
            evhtp_send_reply(req, EVHTP_RES_BADREQ);
            return;
        }

        if (!sv.IsValid()) {
            evhtp_send_reply(req, EVHTP_RES_BADREQ);
            return;
        }
    }
#endif

    try {
        Document d;
        d.Parse(buf, len);
        if (!d.HasMember("id")) {
            evhtp_send_reply(req, EVHTP_RES_BADREQ);
            return;
        }

        const auto& id = d["id"].GetString();
        evbuffer_add(req->buffer_out, "OK:", 3);
        evbuffer_add(req->buffer_out, id, strlen(id));
        evhtp_send_reply(req, EVHTP_RES_OK);
    } catch (...) {
        evhtp_send_reply(req, EVHTP_RES_500);
    }
}

int main() {
    const char* host;
    uint16_t port, num_threads;
    parse_env(&host, &port, &num_threads, &skip_validation);

    const auto path = "share/openrtb-schema_bid-request_v2-3.json";
    const auto content = getFileContent(path);
    Document d;
    d.Parse(content.c_str(), content.size());
    if (d.HasParseError()) {
        fprintf(stderr, "Schema file '%s' is not a valid JSON\n", path);
        return 1;
    }

    if (num_threads > 1) {
        throw std::runtime_error("TODO: current implementation is not thread safe yet.");
    }

    SchemaDocument sd(d);
    SchemaValidator sv(sd);

    evbase_t* evbase = event_base_new();
    evhtp_t* htp = evhtp_new(evbase, NULL);
    evhtp_set_cb(htp, "/add", add_cb, &sv);
    evhtp_use_threads_wexit(htp, init_thread, exit_thread, num_threads, NULL);
    evhtp_bind_socket(htp, host, port, 1024);

    printf("READY\n");

    event_base_loop(evbase, 0);
}

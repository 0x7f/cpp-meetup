#include <rapidjson/document.h>

#include <pbjson.hpp>

#include "bidrequest_parser.pb.h"
#include "evhtp_util.h"
#include "parse_env.h"

using namespace ::com::google::openrtb;

static bool parseRequest(const char* body, BidRequest& msg, std::string& err) {
    rapidjson::Document d;
    d.Parse(body);
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

    std::string err;
    BidRequest bidRequest;
    if (!parseRequest(buf, bidRequest, err)) {
        evbuffer_add(req->buffer_out, err.c_str(), err.size());
        evhtp_send_reply(req, EVHTP_RES_500);
        return;
    }

    const auto& id = bidRequest.id();
    evbuffer_add(req->buffer_out, "OK:", 3);
    evbuffer_add(req->buffer_out, id.c_str(), id.size());
    evhtp_send_reply(req, EVHTP_RES_OK);
}

int main() {
    const char* host;
    uint16_t port, num_threads;
    bool skip_validation;
    parse_env(&host, &port, &num_threads, &skip_validation);

    evbase_t* evbase = event_base_new();
    evhtp_t* htp = evhtp_new(evbase, NULL);
    evhtp_set_cb(htp, "/add", add_cb, NULL);
    evhtp_use_threads_wexit(htp, init_thread, exit_thread, num_threads, NULL);
    evhtp_bind_socket(htp, host, port, 1024);

    printf("READY\n");

    event_base_loop(evbase, 0);
}

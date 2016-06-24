#include <json/reader.h>
#include <json/value.h>

#include "evhtp_util.h"
#include "parse_env.h"

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

    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(buf, buf + len, root, false)) {
        const auto err = reader.getFormattedErrorMessages();
        evbuffer_add(req->buffer_out, err.c_str(), err.size());
        evhtp_send_reply(req, EVHTP_RES_500);
        return;
    }

    const auto& id = root["id"].asString();
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

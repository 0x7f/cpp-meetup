#include <ArduinoJson.h>

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

    StaticJsonBuffer<10 * 1024> jsonBuffer;
    auto& root = jsonBuffer.parseObject(buf);
    if (!root.success()) {
        const auto err = "JsonBuffer::parseObject() failed";
        evbuffer_add(req->buffer_out, err, strlen(err));
        evhtp_send_reply(req, EVHTP_RES_500);
        return;
    }

    const char* id = root["id"];
    evbuffer_add(req->buffer_out, "OK:", 3);
    evbuffer_add(req->buffer_out, id, strlen(id));
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

#pragma once

#include <stdlib.h>
#include <string.h>

#include <evhtp.h>

// TODO: replace with dynamically reallocated buffer
static const int EV_BUF_SIZE = 10 * 1024 * 1024;

static void init_thread(evhtp_t *htp, evthr_t *thread, void *arg) {
    evthr_set_aux(thread, calloc(EV_BUF_SIZE, 1));
}

static void exit_thread(evhtp_t *htp, evthr_t *thread, void *arg) {
    free(evthr_get_aux(thread));
}

static char *extract_body(evhtp_request_t *req, size_t *len) {
  evthr_t *thread = evhtp_request_get_connection(req)->thread;
  char *buf = (char *)evthr_get_aux(thread);
  *len = evbuffer_copyout(req->buffer_in, buf, EV_BUF_SIZE);
  if (*len + 1 >= EV_BUF_SIZE) {
      return NULL;
  }
  buf[*len] = '\0';
  return buf;
}

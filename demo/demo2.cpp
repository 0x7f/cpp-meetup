#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "bidrequest_parser.pb.h"

using namespace ::com::google::openrtb;

int main() {
    BidRequest bidRequest;
    bidrequest_parser_state_t state = bidrequest_parser_init(bidRequest);

    char buf[1024];
    int rc, bytesRead;
    while((bytesRead = read(STDIN_FILENO, buf, sizeof(buf) / sizeof(buf[0]))) > 0) {
        rc = bidrequest_parser_on_chunk(state, buf, bytesRead);
        if (rc != 0) {
            char* err = bidrequest_parser_get_error(state);
            fprintf(stderr, "Error: %s\n", err);
            bidrequest_parser_free_error(state, err);
            return EXIT_FAILURE;
        }
    }

    rc = bidrequest_parser_complete(state);
    if (rc != 0) {
        char* err = bidrequest_parser_get_error(state);
        fprintf(stderr, "Error: %s\n", err);
        bidrequest_parser_free_error(state, err);
        return EXIT_FAILURE;
    }

    bidrequest_parser_free(state);

    printf("%s\n", bidRequest.DebugString().c_str());

    return EXIT_SUCCESS;
}

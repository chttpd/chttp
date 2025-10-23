// Copyright 2025 Vahid Mardani
/*
 * This file is part of chttp.
 *  chttp is free software: you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation, either version 3 of the License, or (at your option)
 *  any later version.
 *
 *  chttp is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with chttp. If not, see <https://www.gnu.org/licenses/>.
 *
 *  Author: Vahid Mardani <vahid.mardani@gmail.com>
 */
/* standard */
#include <stdlib.h>
#include <string.h>

/* thirdparty */
#include <cutest.h>

/* local public */
#include "chttp.h"


void
test_responsemaker_start() {
    struct chttp_request *r = chttp_request_new(3);
    struct chttp_responsemaker *resp = &r->response;
    isnotnull(r);

    memset(&r->response, 0, sizeof(r->response));
    eqint(0, chttp_responsemaker_start(r, CHTTP_STATUS_200_OK, NULL));
    eqint(0, chttp_responsemaker_header_close(r));
    eqnstr("HTTP/1.1 200 Ok\r\n\r\n", resp->header, resp->headerlen);

    memset(resp, 0, sizeof(r->response));
    eqint(0, chttp_responsemaker_start(r, CHTTP_STATUS_200_OK, "Foo"));
    eqint(0, chttp_responsemaker_header_close(r));
    eqnstr("HTTP/1.1 200 Foo\r\n\r\n", resp->header, resp->headerlen);

    eqint(-1, chttp_responsemaker_start(r, 0, NULL));

    chttp_request_free(r);
}


void
test_responsemaker_headers() {
    struct chttp_request *r = chttp_request_new(3);
    struct chttp_responsemaker *resp = &r->response;
    isnotnull(r);

    eqint(-1, chttp_responsemaker_header(r, "foo = %s", "bar"));

    eqint(0, chttp_responsemaker_start(r, CHTTP_STATUS_200_OK, NULL));
    eqint(0, chttp_responsemaker_header(r, "foo = %s", "bar"));
    eqint(0, chttp_responsemaker_contenttype(r, "text/plain", "utf-8"));
    eqint(0, chttp_responsemaker_header_close(r));
    eqnstr("HTTP/1.1 200 Ok\r\n"
            "foo = bar\r\n"
            "Content-Type: text/plain; charset=utf-8\r\n\r\n",
            resp->header, resp->headerlen);

    chttp_request_free(r);
}


void
test_responsemaker_content() {
    struct chttp_request *r = chttp_request_new(3);
    struct chttp_responsemaker *resp = &r->response;
    isnotnull(r);

    eqint(0, chttp_responsemaker_start(r, CHTTP_STATUS_200_OK, NULL));
    eqint(0, chttp_responsemaker_content_allocate(r, 128));
    eqint(7, chttp_responsemaker_content_write(r, "foo %s", "bar"));
    eqint(9, chttp_responsemaker_content_write(r, " baz %s", "quux"));

    eqint(0, chttp_responsemaker_header_close(r));
    eqnstr("HTTP/1.1 200 Ok\r\n"
            "Content-Length: 16\r\n\r\n",
            resp->header, resp->headerlen);
    eqnstr("foo bar baz quux", r->response.content,
            r->response.contentlength);

    chttp_request_free(r);
}


int
main() {
    test_responsemaker_content();
    test_responsemaker_headers();
    test_responsemaker_start();
    return EXIT_SUCCESS;
}

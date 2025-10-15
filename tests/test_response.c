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
test_response_start() {
    char buff[1024];
    struct chttp_request *r = chttp_request_new(3);
    isnotnull(r);

    memset(&r->response, 0, sizeof(r->response));
    eqint(0, chttp_response_start(r, CHTTP_STATUS_200_OK, NULL));
    eqint(19, chttp_response_tobuff(r, buff, sizeof(buff)));
    eqnstr("HTTP/1.1 200 Ok\r\n\r\n", buff, 19);

    memset(&r->response, 0, sizeof(r->response));
    eqint(0, chttp_response_start(r, CHTTP_STATUS_200_OK, "Foo"));
    eqint(20, chttp_response_tobuff(r, buff, sizeof(buff)));
    eqnstr("HTTP/1.1 200 Foo\r\n\r\n", buff, 20);

    eqint(-1, chttp_response_start(r, 0, NULL));

    free(r);
}


void
test_response_headers() {
    char buff[1024];
    struct chttp_request *r = chttp_request_new(3);
    isnotnull(r);

    eqint(-1, chttp_response_header(r, "foo = %s", "bar"));

    eqint(0, chttp_response_start(r, CHTTP_STATUS_200_OK, NULL));
    eqint(0, chttp_response_header(r, "foo = %s", "bar"));
    eqint(0, chttp_response_contenttype(r, "text/plain", "utf-8"));
    eqint(71, chttp_response_tobuff(r, buff, sizeof(buff)));
    eqnstr("HTTP/1.1 200 Ok\r\n"
            "foo = bar\r\n"
            "Content-Type: text/plain; charset=utf-8\r\n\r\n", buff, 71);

    free(r);
}


int
main() {
    test_response_headers();
    test_response_start();
    return EXIT_SUCCESS;
}

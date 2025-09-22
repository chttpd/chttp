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
/* thirdparty */
#include <cutest.h>

/* local private */
#include "request.h"

/* local public */
#include "chttp.h"


void
test_request_boundary() {
    struct chttp_request req;

    eqint(400, request_fromstring(&req, "GET / HTTP/1.1\nfoo=bar\n\n\n"));
    eqint(400, request_fromstring(&req, "GET / HTTP/1.1\r\n\r\n\r\n"));
    eqint(400, request_fromstring(&req, "GET / HTTP/1.1\r\n"));
    eqint(400, request_fromstring(&req, "GET / HTTP/1.1\n\n\n"));
    eqint(400, request_fromstring(&req, "GET / HTTP/1.1"));
    eqint(0, request_fromstring(&req, "GET / HTTP/1.1\n\n"));
}


void
test_request_startline_parse() {
    struct chttp_request req;
    memset(&req, 0, sizeof(req));

    eqint(0, request_fromstring(&req,
                "GET /foo?bar=baz%%20qux HTTP/1.1\r\n\r\n"));
    eqstr("GET", req.verb);
    eqstr("/foo", req.path);
    eqstr("bar=baz qux", req.query);
    eqstr("HTTP/1.1", req.protocol);
}


void
test_request_headers_parse() {
    struct chttp_request req;
    memset(&req, 0, sizeof(req));

    eqint(0, request_fromstring(&req, "GET / HTTP/1.1\n\n"));
    eqint(0, req.headerscount);

    eqint(0, request_fromstring(&req, "GET / HTTP/1.1\nfoo = bar\n\n"));
    eqstr("foo = bar", req.headers[0]);
    eqint(1, req.headerscount);
}


int
main() {
    test_request_startline_parse();
    test_request_boundary();
    test_request_headers_parse();
    return EXIT_SUCCESS;
}

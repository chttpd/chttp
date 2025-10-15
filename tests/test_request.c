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
#include <unistd.h>

/* thirdparty */
#include <cutest.h>

/* local public */
#include "chttp.h"

/* local private */
#include "fixtures.h"


static void
test_request_new() {
    struct chttp_request *r;

    r = chttp_request_new(3);
    isnotnull(r);
    eqint(-1, r->contentlength);
    free(r);
}


static void
test_request_startline() {
    struct chttp_request *r = chttp_request_new(3);
    isnotnull(r);

    eqint(0, requestf(r, "GET / HTTP/1.1\r\n"));
    eqstr("GET", r->verb);
    eqstr("/", r->path);
    eqstr("HTTP/1.1", r->protocol);

    eqint(0, requestf(r, "GET /?foo HTTP/1.1\r\n"));
    eqstr("/", r->path);
    eqstr("foo", r->query);

    eqint(400, requestf(r, "GET ?foo HTTP/1.1\r\n"));
    eqint(0, requestf(r, "GET //foo HTTP/1.1\r\n"));
    eqstr("//foo", r->path);

    eqint(0, requestf(r, "GET /foo/bar HTTP/1.1\r\n"));
    eqstr("/foo/bar", r->path);

    eqint(400, requestf(r, "GET /foo/bar HTTP/1.1\r\n\r\n"));
    free(r);
}


static void
test_request_headers_parse() {
    struct chttp_request *r = chttp_request_new(3);
    isnotnull(r);

    eqint(0, requestf(r, "GET /foo/bar HTTP/1.1\r\n"));
    eqint(0, r->headerscount);
    eqint(-1, r->contentlength);

    eqint(0, requestf(r, "GET /foo/bar HTTP/1.1\r\nfoo = bar\r\n"));
    eqstr("foo = bar", r->headers[0]);
    eqint(1, r->headerscount);

    eqint(0, requestf(r, "GET / HTTP/1.1\r\n"
                "content-type: text/plain; charset=utf-8\r\n"
                "content-length: 12\r\n"
                "expect: 100-continue\r\n"
                "user-agent: foo bar baz\r\n"
                "foo: bar\r\n"));
    eqstr("text/plain", r->contenttype);
    eqstr("utf-8", r->charset);
    eqint(12, r->contentlength);
    eqstr("100-continue", r->expect);
    eqstr("foo bar baz", r->useragent);
    eqint(1, r->headerscount);
    eqstr("foo: bar", r->headers[0]);

    free(r);
}


// void
// test_request() {
//     struct chttp_request req;
//
//     eqint(0, chttp_request_fromstring(&req, "GET / HTTP/1.1\n\n"));
//
// }
// void
//     struct chttp_request req;
//
//
// }


int
main() {
    test_request_headers_parse();
    test_request_startline();
    test_request_new();
    return EXIT_SUCCESS;
}

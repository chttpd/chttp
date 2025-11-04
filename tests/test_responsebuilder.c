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
test_responsebuilder_start() {
    struct chttp_responsebuilder r;

    eqint(0, chttp_responsebuilder_allocate(&r, 1, 1));
    eqint(0, chttp_responsebuilder_start(&r, CHTTP_STATUS_200_OK, NULL));
    eqnstr("HTTP/1.1 200 Ok\r\n", r.header, r.headerlen);

    chttp_responsebuilder_reset(&r);
    eqint(0, chttp_responsebuilder_start(&r, CHTTP_STATUS_200_OK, "foo"));
    eqnstr("HTTP/1.1 200 foo\r\n", r.header, r.headerlen);

    eqint(-1, chttp_responsebuilder_start(&r, 0, NULL));
    eqint(-1, chttp_responsebuilder_start(&r, 0, "foo"));

    chttp_responsebuilder_free(&r);
}


void
test_responsebuilder_headers() {
    struct chttp_responsebuilder r;

    eqint(0, chttp_responsebuilder_allocate(&r, 1, 1));
    eqint(0, chttp_responsebuilder_start(&r, CHTTP_STATUS_200_OK, NULL));
    eqint(0, chttp_responsebuilder_header(&r, "foo = %s", "bar"));
    eqint(0, chttp_responsebuilder_contenttype(&r, "text/plain", "utf-8"));
    eqint(0, chttp_responsebuilder_close(&r));
    eqnstr("HTTP/1.1 200 Ok\r\n"
            "foo = bar\r\n"
            "Content-Type: text/plain; charset=utf-8\r\n"
            "Content-Length: 0\r\n"
            "\r\n",
             r.header, r.headerlen);

    chttp_responsebuilder_free(&r);
}


void
test_responsebuilder_content() {
    struct chttp_responsebuilder r;

    eqint(0, chttp_responsebuilder_allocate(&r, 1, 1));
    eqint(0, chttp_responsebuilder_start(&r, CHTTP_STATUS_200_OK, NULL));
    eqint(0, chttp_responsebuilder_write(&r, "foo %s ", "bar"));
    eqint(0, chttp_responsebuilder_write(&r, "baz %s", "qux"));
    eqint(0, chttp_responsebuilder_close(&r));
    eqnstr("HTTP/1.1 200 Ok\r\n"
            "Content-Length: 15\r\n"
            "\r\n",
             r.header, r.headerlen);
    eqnstr("foo bar baz qux", r.content, r.contentlen);
    chttp_responsebuilder_free(&r);
}


int
main() {
    test_responsebuilder_content();
    test_responsebuilder_headers();
    test_responsebuilder_start();
    return EXIT_SUCCESS;
}

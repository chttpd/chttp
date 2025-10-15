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

    eqint(0, chttp_response_start(r, CHTTP_STATUS_200_OK, NULL));
    eqint(19, chttp_response_tobuff(r, buff, sizeof(buff)));
    eqnstr("HTTP/1.1 200 Ok\r\n\r\n", buff, 19);

    // chttp_response_start(r, 200, NULL);
    // chttp_response_contenttype(r, "text/plain);
    // chttp_response_header(r, "foo", "bar);
    // chttp_response_body(r, "foo %s", "bar");
    // chttp_response_tobuff(r, "foo %s", "bar");

    // eqint(0, chttp_response_header(&resp, "foo = %s", "bar"));
    // resp.contentlength = 0;
    // eqint(0, chttp_response_contenttype(&resp, "text/plain", "utf-8"));
    // len = chttp_response_tobuff(&resp, buff, &bufflen);
    // eqint(90, len);
    // eqstr("HTTP/1.1 200 Ok\r\n"
    //         "Content-Length: 0\r\n"
    //         "Content-Type: text/plain; charset=utf-8\r\n"
    //         "foo = bar\r\n\r\n", buff);

    // eqint(9, chttp_response_write(&resp, "foo %s\r\n", "bar"));
    // len = chttp_response_tobuff(&resp, buff, &bufflen);
    // eqint(99, len);
    // eqnstr("HTTP/1.1 200 Ok\r\n"
    //         "Content-Length: 9\r\n"
    //         "Content-Type: text/plain; charset=utf-8\r\n"
    //         "foo = bar\r\n\r\n"
    //         "foo bar\r\n", buff, len);
}


int
main() {
    test_response_start();
    return EXIT_SUCCESS;
}

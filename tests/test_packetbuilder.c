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
test_packetbuilder_start() {
    struct chttp_packet p;

    eqint(0, chttp_packetbuilder_allocate(&p, 1, 1));
    eqint(0, chttp_packetbuilder_startresponse(&p, CHTTP_STATUS_200_OK,
                NULL));
    eqnstr("HTTP/1.1 200 Ok\r\n", p.header, p.headerlen);

    chttp_packetbuilder_reset(&p);
    eqint(0, chttp_packetbuilder_startresponse(&p, CHTTP_STATUS_200_OK,
                "foo"));
    eqnstr("HTTP/1.1 200 foo\r\n", p.header, p.headerlen);

    eqint(-1, chttp_packetbuilder_startresponse(&p, 0, NULL));
    eqint(-1, chttp_packetbuilder_startresponse(&p, 0, "foo"));

    chttp_packetbuilder_free(&p);
}


void
test_packetbuilder_headers() {
    struct chttp_packet p;

    eqint(0, chttp_packetbuilder_allocate(&p, 1, 1));
    eqint(0, chttp_packetbuilder_startresponse(&p, CHTTP_STATUS_200_OK,
                NULL));
    eqint(0, chttp_packetbuilder_header(&p, "foo = %s", "bar"));
    eqint(0, chttp_packetbuilder_contenttype(&p, "text/plain", "utf-8"));
    eqint(0, chttp_packetbuilder_close(&p));
    eqnstr("HTTP/1.1 200 Ok\r\n"
            "foo = bar\r\n"
            "Content-Type: text/plain; charset=utf-8\r\n"
            "Content-Length: 0\r\n"
            "\r\n",
             p.header, p.headerlen);

    chttp_packetbuilder_free(&p);
}


void
test_packetbuilder_content() {
    struct chttp_packet p;

    eqint(0, chttp_packetbuilder_allocate(&p, 1, 1));
    eqint(0, chttp_packetbuilder_startresponse(&p, CHTTP_STATUS_200_OK,
                NULL));
    eqint(0, chttp_packetbuilder_write(&p, "foo %s ", "bar"));
    eqint(0, chttp_packetbuilder_write(&p, "baz %s", "qux"));
    eqint(0, chttp_packetbuilder_close(&p));
    eqnstr("HTTP/1.1 200 Ok\r\n"
            "Content-Length: 15\r\n"
            "\r\n",
             p.header, p.headerlen);
    eqnstr("foo bar baz qux", p.content, p.contentlen);
    chttp_packetbuilder_free(&p);
}


int
main() {
    test_packetbuilder_content();
    test_packetbuilder_headers();
    test_packetbuilder_start();
    return EXIT_SUCCESS;
}

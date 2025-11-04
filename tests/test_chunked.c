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


static void
test_chunked_malformed() {
    char *in = "3\r\nfooXX0\r\n\r\n";
    const char *chunk;
    size_t plen;

    /* more data needed */
    in = "foo";
    eqint(-2, chttp_chunked_parse(in, strlen(in), &chunk, &plen));

    in = "3\rXfoo";
    eqint(-2, chttp_chunked_parse(in, strlen(in), &chunk, &plen));

    /* malformed */
    in = "3\r\nfooXX\n0\r\n\r\n";
    eqint(-1, chttp_chunked_parse(in, strlen(in), &chunk, &plen));

    in = "3X\r\nfoo\r\n0\r\n\r\n";
    eqint(-1, chttp_chunked_parse(in, strlen(in), &chunk, &plen));

    in = "3XX\nfoo\r\n0\r\n\r\n";
    eqint(-1, chttp_chunked_parse(in, strlen(in), &chunk, &plen));

    in = "XX\r\nfoo\r\n0\r\n\r\n";
    eqint(-1, chttp_chunked_parse(in, strlen(in), &chunk, &plen));
}


static void
test_chunked() {
    char *input = "3\r\nfoo\r\n0\r\n\r\n";
    char *cursor = input;
    const char *chunk;
    size_t len = strlen(input);
    ssize_t chunklen;
    size_t plen;

    chunklen = chttp_chunked_parse(cursor, len, &chunk, &plen);
    eqint(3, chunklen);
    eqint(8, plen);
    eqnstr("foo", chunk, chunklen);

    cursor += plen;
    len -= plen;
    chunklen = chttp_chunked_parse(cursor, len, &chunk, &plen);
    eqint(0, chunklen);
    eqint(5, plen);
}


int
main() {
    test_chunked_malformed();
    test_chunked();
    return EXIT_SUCCESS;
}

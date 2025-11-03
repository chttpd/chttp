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
    size_t garbage;

    /* more data needed */
    in = "foo";
    eqint(-2, chttp_chunkedcodec_getchunk(in, strlen(in), &chunk, &garbage));

    in = "3\rXfoo";
    eqint(-2, chttp_chunkedcodec_getchunk(in, strlen(in), &chunk, &garbage));

    /* malformed */
    in = "3\r\nfooXX\n0\r\n\r\n";
    eqint(-1, chttp_chunkedcodec_getchunk(in, strlen(in), &chunk, &garbage));

    in = "3X\r\nfoo\r\n0\r\n\r\n";
    eqint(-1, chttp_chunkedcodec_getchunk(in, strlen(in), &chunk, &garbage));

    in = "3XX\nfoo\r\n0\r\n\r\n";
    eqint(-1, chttp_chunkedcodec_getchunk(in, strlen(in), &chunk, &garbage));

    in = "XX\r\nfoo\r\n0\r\n\r\n";
    eqint(-1, chttp_chunkedcodec_getchunk(in, strlen(in), &chunk, &garbage));
}


static void
test_chunked() {
    char *input = "3\r\nfoo\r\n0\r\n\r\n";
    char *cursor = input;
    const char *chunk;
    size_t len = strlen(input);
    ssize_t chunklen;
    size_t garbage;

    chunklen = chttp_chunkedcodec_getchunk(cursor, len, &chunk, &garbage);
    eqint(3, chunklen);
    eqint(8, garbage);
    eqnstr("foo", chunk, chunklen);

    cursor += garbage;
    len -= garbage;
    chunklen = chttp_chunkedcodec_getchunk(cursor, len, &chunk, &garbage);
    eqint(0, chunklen);
    eqint(5, garbage);
}


int
main() {
    test_chunked_malformed();
    test_chunked();
    return EXIT_SUCCESS;
}

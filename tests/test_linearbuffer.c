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
#include "linearbuffer.h"


#define BUFFSIZE  512


void
test_linearbuffer() {
    char backend[BUFFSIZE];
    struct linearbuffer b;

    linearbuffer_init(&b, backend, BUFFSIZE);
    eqint(BUFFSIZE, b.size);
    eqint(0, b.len);
    eqptr(backend, b.backend);

    eqptr(backend, linearbuffer_allocate(&b, "foo bar"));
    eqint(8, b.len);

    eqptr(backend + 8, linearbuffer_allocate(&b, "baz"));
    eqint(12, b.len);
}


void
test_linearbuffer_splitallocate() {
    char backend[BUFFSIZE];
    struct linearbuffer b;
    char in[16];
    char *out[8];

    linearbuffer_init(&b, backend, BUFFSIZE);

    strcpy(in, "foo\nbar\n\n");
    eqint(-2, linearbuffer_splitallocate(&b, in, "\n", out, 8));

    strcpy(in, "\r\n\r\n");
    eqint(-2, linearbuffer_splitallocate(&b, in, "\n", out, 8));
    eqint(-2, linearbuffer_splitallocate(&b, "\n\n", "\n", out, 8));
    eqint(-2, linearbuffer_splitallocate(&b, "\n", "\n", out, 8));
    eqint(0, linearbuffer_splitallocate(&b, "", "\n", out, 8));

    strcpy(in, "foo bar");
    eqint(2, linearbuffer_splitallocate(&b, in, " ", out, 8));
}


int
main() {
    test_linearbuffer_splitallocate();
    test_linearbuffer();
    return EXIT_SUCCESS;
}

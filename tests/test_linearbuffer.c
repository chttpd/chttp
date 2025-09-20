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

    eqptr(backend, linearbuffer_allocate(&b, "foo bar", 7));
    eqint(8, b.len);

    eqptr(backend + 8, linearbuffer_unsafeallocate(&b, "baz"));
    eqint(12, b.len);
}


int
main() {
    test_linearbuffer();
    return EXIT_SUCCESS;
}

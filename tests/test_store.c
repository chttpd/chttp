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
#include "store.h"


#define BUFFSIZE  512


void
test_store_replaceall() {
    char buff[16] = "foo\0bar\0";
    char backend[BUFFSIZE];
    struct chttp_store b;
    char *foo = buff;
    char *bar = buff + 4;
    char **both[2] = {&foo, &bar};

    store_init(&b, backend, BUFFSIZE);
    eqint(0, store_replaceall(&b, 2, both));
    eqstr("foo", foo);
    eqptr(backend, foo);
    eqstr("bar", bar);
    eqptr(backend + 4, bar);
    eqbin("foo\0bar\0", backend, 8);
}


void
test_store_replace() {
    char buff[8] = "foo\0";
    char backend[BUFFSIZE];
    struct chttp_store b;
    char *foo = buff;

    store_init(&b, backend, BUFFSIZE);
    eqint(0, store_replace(&b, &foo));
    eqstr("foo", foo);
    eqptr(backend, foo);
}


void
test_store() {
    char backend[BUFFSIZE];
    struct chttp_store b;

    store_init(&b, backend, BUFFSIZE);
    eqint(BUFFSIZE, b.size);
    eqint(0, b.len);
    eqptr(backend, b.backend);

    eqptr(backend, store_allocate(&b, "foo bar"));
    eqint(8, b.len);

    eqptr(backend + 8, store_allocate(&b, "baz"));
    eqint(12, b.len);
}


int
main() {
    test_store_replaceall();
    test_store_replace();
    test_store();
    return EXIT_SUCCESS;
}

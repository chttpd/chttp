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


// void
// test_store_replaceall() {
//     char buff[16] = "foo\0bar\0";
//     char backend[BUFFSIZE];
//     struct chttp_store b;
//     const char *foo = buff;
//     const char *bar = buff + 4;
//     const char **both[2] = {&foo, &bar};
//
//     store_init(&b, backend, BUFFSIZE);
//     eqint(0, store_replaceall(&b, 2, both));
//     eqstr("foo", foo);
//     eqptr(backend, foo);
//     eqstr("bar", bar);
//     eqptr(backend + 4, bar);
//     eqbin("foo\0bar\0", backend, 8);
// }
//
//
// void
// test_store_replace() {
//     char buff[8] = "foo\0";
//     char backend[BUFFSIZE];
//     struct chttp_store b;
//     const char *foo = buff;
//
//     store_init(&b, backend, BUFFSIZE);
//     eqint(0, store_replace(&b, &foo));
//     eqstr("foo", foo);
//     eqptr(backend, foo);
// }

// void
// test_store_ifstartswith_ci() {
//     char backend[BUFFSIZE];
//     struct chttp_store b;
//     const char *dst;
//
//     store_init(&b, backend, BUFFSIZE);
//
//     eqint(-1, store_ifstartswith_ci(&b, &dst, "foo bar", NULL));
//     eqint(-1, store_ifstartswith_ci(&b, &dst, "foo bar", ""));
//
//     eqint(0, store_ifstartswith_ci(&b, &dst, "foo bar", "bar"));
// }


void
test_store_str() {
    char backend[16];
    struct chttp_store b;
    const char *dst = "foo";

    store_init(&b, backend, sizeof(backend));

    eqint(0, store_str(&b, &dst, NULL));
    isnull(dst);

    eqint(0, store_str(&b, &dst, "foo bar"));
    eqptr(backend, dst);
    eqstr("foo bar", dst);
    eqint(8, b.len);

    eqint(0, store_str(&b, &dst, "baz"));
    eqptr(backend + 8, dst);
    eqstr("baz", dst);
    eqint(12, b.len);

    eqint(0, store_str(&b, &dst, "qux"));
    eqstr("qux", dst);

    eqint(-1, store_str(&b, &dst, "1"));
}


void
test_store_allocate() {
    char backend[16];
    struct chttp_store b;

    store_init(&b, backend, sizeof(backend));

    /* allocate zero bytes! */
    isnull(store_allocate(&b, 0));

    /* sucessfull */
    eqptr(backend, store_allocate(&b, 10));
    eqint(10, b.len);
    eqptr(backend + 10, store_allocate(&b, 6));
    eqint(16, b.len);

    /* insufficient */
    isnull(store_allocate(&b, 1));
    isnull(store_allocate(&b, 2));
}


void
test_store_init() {
    char backend[BUFFSIZE];
    struct chttp_store b;

    store_init(&b, backend, BUFFSIZE);
    eqint(BUFFSIZE, b.size);
    eqint(0, b.len);
    eqptr(backend, b.backend);
}


int
main() {
    // TODO: test_store_all();
    // test_store_ifstartswith_ci();
    test_store_str();
    test_store_allocate();
    test_store_init();
    return EXIT_SUCCESS;
}

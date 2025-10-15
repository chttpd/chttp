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


#define BUFFSIZE  16


void
test_store_appendf() {
    char backend[BUFFSIZE];
    struct chttp_store b;
    const char *dst;

    store_init(&b, backend, BUFFSIZE);

    eqint(-1, store_appendf(&b, "foo"));
    eqint(0, b.len);

    eqint(0, store_str(&b, &dst, NULL, "foo bar"));
    eqstr("foo bar", dst);
    eqint(8, b.len);

    eqint(0, store_appendf(&b, " %s", "baz"));
    eqstr("foo bar baz", dst);
    eqint(12, b.len);

    eqint(0, store_appendf(&b, " %s", "qux"));
    eqstr("foo bar baz qux", dst);
    eqint(16, b.len);

    eqint(-1, store_appendf(&b, "x"));
    eqstr("foo bar baz qux", dst);
    eqint(16, b.len);
}


void
test_store_strf() {
    char backend[BUFFSIZE];
    struct chttp_store b;
    const char *dst = "foo";

    store_init(&b, backend, sizeof(backend));

    eqint(0, store_strf(&b, &dst, NULL));
    isnull(dst);

    eqint(0, store_strf(&b, &dst, "foo %s", "bar"));
    eqstr("foo bar", dst);
    eqint(8, b.len);

    eqint(-1, store_strf(&b, &dst, "baz qux %s", "quux"));
    eqstr("foo bar", dst);
    eqint(8, b.len);
}


void
test_store_append() {
    char backend[BUFFSIZE];
    struct chttp_store b;
    const char *dst;

    store_init(&b, backend, BUFFSIZE);

    eqint(-1, store_append(&b, "foo"));
    eqint(0, b.len);

    eqint(0, store_str(&b, &dst, NULL, "foo bar"));
    eqstr("foo bar", dst);
    eqint(8, b.len);

    eqint(0, store_append(&b, " baz"));
    eqstr("foo bar baz", dst);
    eqint(12, b.len);

    eqint(0, store_append(&b, " qux"));
    eqstr("foo bar baz qux", dst);
    eqint(16, b.len);

    eqint(-1, store_append(&b, "x"));
    eqstr("foo bar baz qux", dst);
    eqint(16, b.len);
}


void
test_store_suffixifprefix_ci() {
    char backend[BUFFSIZE];
    struct chttp_store b;
    const char *dst;

    store_init(&b, backend, BUFFSIZE);

    eqint(-1, store_suffixifprefix_ci(&b, &dst, NULL, "foo bar", NULL));
    eqint(-1, store_suffixifprefix_ci(&b, &dst, NULL, "foo bar", ""));

    eqint(1, store_suffixifprefix_ci(&b, &dst, NULL, "foo bar", "bar"));

    eqint(0, store_suffixifprefix_ci(&b, &dst, NULL, "foo bar baz", "foo "));
    eqptr(backend, dst);
    eqstr("bar baz", dst);
    eqint(8, b.len);
    eqint(0, store_suffixifprefix_ci(&b, &dst, NULL, "bar baz", "bar"));

    /* auto trim */
    eqint(0, store_suffixifprefix_ci(&b, &dst, NULL, "foo bar", "foo"));
    eqstr("bar", dst);

    /* insufficient space */
    eqint(-1, store_suffixifprefix_ci(&b, &dst, NULL, "foo bar", "foo"));
}


void
test_store_all() {
    char backend[BUFFSIZE];
    struct chttp_store b;
    const char *srcs[5] = {"foo", "bar", "baz", "qux", "quux"};
    const char *foo, *bar, *baz, *qux, *quux;
    const char **dsts[5] = {&foo, &bar, &baz, &qux, &quux};

    store_init(&b, backend, sizeof(backend));
    eqint(4, store_all(&b, 4, dsts, srcs));
    eqstr("foo", foo);
    eqstr("bar", bar);
    eqstr("baz", baz);
    eqstr("qux", qux);

    store_init(&b, backend, sizeof(backend));
    eqint(4, store_all(&b, 5, dsts, srcs));
    eqint(0, store_all(&b, 5, dsts, srcs));
}


void
test_store_str() {
    char backend[BUFFSIZE];
    struct chttp_store b;
    const char *dst = "foo";
    size_t len;

    store_init(&b, backend, sizeof(backend));

    eqint(0, store_str(&b, &dst, NULL, NULL));
    eqint(0, store_str(&b, &dst, &len, NULL));
    isnull(dst);
    eqint(0, len);

    eqint(0, store_str(&b, &dst, &len, "foo bar"));
    eqptr(backend, dst);
    eqstr("foo bar", dst);
    eqint(8, b.len);
    eqint(7, len);

    eqint(0, store_str(&b, &dst, &len, "baz"));
    eqptr(backend + 8, dst);
    eqstr("baz", dst);
    eqint(12, b.len);
    eqint(4, len);

    eqint(0, store_str(&b, &dst, &len, "qux"));
    eqstr("qux", dst);
    eqint(3, len);

    eqint(-1, store_str(&b, &dst, NULL, "1"));
}


void
test_store_allocate() {
    char backend[BUFFSIZE];
    struct chttp_store b;

    store_init(&b, backend, sizeof(backend));

    /* allocate zero bytes! */
    isnull(store_allocate(&b, 0));

    /* sucessfull */
    eqptr(backend, store_allocate(&b, 10));
    eqint(10, b.len);
    eqptr(backend + 10, store_allocate(&b, 6));
    eqint(BUFFSIZE, b.len);

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
    // test_store_suffixifprefix_ci();
    // test_store_all();
    // test_store_appendf();
    // test_store_append();
    // test_store_strf();
    test_store_str();
    test_store_allocate();
    test_store_init();
    return EXIT_SUCCESS;
}

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

/* thirdparty */
#include <cutest.h>

/* local private */
#include "str.h"


static void
test_strsplit() {
    char tmp[32];
    char *first = NULL;
    char *second = NULL;

    /* null */
    eqint(-1, strsplit(NULL, " ", (char **[]){&first, &second}, 2));
    isnull(first);
    isnull(second);

    /* empty */
    first = NULL;
    second = NULL;
    eqint(0, strsplit("", " ", (char **[]){&first, &second}, 2));
    eqstr("", first);
    isnull(second);

    /* no delimiter inside input string */
    first = NULL;
    second = NULL;
    sprintf(tmp, "foobar");
    eqint(1, strsplit(tmp, " ", (char **[]){&first, &second}, 2));
    eqstr("foobar", first);
    isnull(second);

    /* extra delimiter */
    first = NULL;
    second = NULL;
    sprintf(tmp, "foo bar ");
    eqint(2, strsplit(tmp, " ", (char **[]){&first, &second}, 2));
    eqstr("foo", first);
    eqstr("bar", second);

    /* insufficient token */
    first = NULL;
    second = NULL;
    sprintf(tmp, "foo bar");
    eqint(2, strsplit(tmp, " ", (char **[]){&first, &second}, 3));
    eqstr("foo", first);
    eqstr("bar", second);

    /* extra token */
    first = NULL;
    second = NULL;
    sprintf(tmp, "foo bar baz");
    eqint(-1, strsplit(tmp, " ", (char **[]){&first, &second}, 2));
    eqstr("foo", first);
    eqstr("bar", second);

    /* as expected */
    first = NULL;
    second = NULL;
    sprintf(tmp, "foo bar");
    eqint(2, strsplit(tmp, " ", (char **[]){&first, &second}, 2));
    eqstr("foo", first);
    eqstr("bar", second);
}


static void
test_strtrim() {
    char tmp[16];

    sprintf(tmp, " foo ");
    eqstr("foo", strtrim(tmp));

    sprintf(tmp, "foo\r");
    eqstr("foo", strtrim(tmp));
}


int
main() {
    test_strsplit();
    test_strtrim();
    return EXIT_SUCCESS;
}

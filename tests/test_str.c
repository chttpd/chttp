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
test_strtoktrim() {
    char *saveptr;
    char in[16];

    sprintf(in, "\n");
    eqptr(in, strtoktrim(in, "\n", &saveptr));
    eqint(0, in[0]);
    eqptr(in + 1, saveptr);
    isnull(strtoktrim(NULL, "\n", &saveptr));

    sprintf(in, "foo\r\nbar");
    eqptr(in, strtoktrim(in, "\n", &saveptr));
    eqint(0, in[3]);
    eqptr(in + 5, saveptr);
    eqstr("bar", strtoktrim(NULL, "\n", &saveptr));
    eqint(0, in[8]);
    eqptr(in + 8, saveptr);
    isnull(strtoktrim(NULL, "\n", &saveptr));
    eqptr(in + 8, saveptr);
}


static void
test_strsplit() {
    char tmp[32];
    char *out[2];

    /* no delimiter inside input string */
    sprintf(tmp, "foobar");
    eqint(1, strsplit(tmp, " ", 2, out));
    eqstr("foobar", out[0]);

    /* null */
    eqint(-1, strsplit(NULL, " ", 2, out));

    /* empty */
    eqint(0, strsplit("", " ", 2, out));

    /* extra delimiter */
    sprintf(tmp, "foo bar ");
    eqint(2, strsplit(tmp, " ", 2, out));
    eqstr("foo", out[0]);
    eqstr("bar", out[1]);

    /* insufficient token */
    sprintf(tmp, "foo bar ");
    eqint(2, strsplit(tmp, " ", 3, out));
    eqstr("foo", out[0]);
    eqstr("bar", out[1]);

    /* extra token */
    sprintf(tmp, "foo bar baz");
    eqint(-2, strsplit(tmp, " ", 2, out));
    eqstr("foo", out[0]);
    eqstr("bar", out[1]);

    /* as expected */
    sprintf(tmp, "foo bar");
    eqint(2, strsplit(tmp, " ", 2, out));
    eqstr("foo", out[0]);
    eqstr("bar", out[1]);
}


static void
test_strtrim() {
    char tmp[16];
    int len;

    sprintf(tmp, "\r");
    eqint(1, strlen(tmp));
    eqstr("", strtrim(tmp, &len));
    eqint(0, len);

    sprintf(tmp, " foo ");
    eqstr("foo", strtrim(tmp, &len));
    eqint(3, len);

    sprintf(tmp, "foo\r");
    eqstr("foo", strtrim(tmp, &len));
    eqint(3, len);
}


int
main() {
    test_strtoktrim();
    test_strsplit();
    test_strtrim();
    return EXIT_SUCCESS;
}

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
#include "buffwriter.h"


void
test_buffwriter_printf() {
    char buff[16];
    struct buffwriter w = {buff, sizeof(buff), 0};

    eqint(8, buffwriter_printf(&w, "foo %s", "bar"));
    eqint(7, w.used);

    eqint(4, buffwriter_printf(&w, " baz"));
    eqint(11, w.used);

    eqint(-1, buffwriter_printf(&w, " thud"));
    eqint(11, w.used);
}


void
test_buffwriter_write() {
    char buff[16];
    struct buffwriter w = {buff, sizeof(buff), 0};

    eqint(4, buffwriter_write(&w, "foo bar baz", 11));
    eqint(11, w.used);

    eqint(-1, buffwriter_write(&w, " thud", 5));
    eqint(11, w.used);
}


int
main() {
    test_buffwriter_write();
    test_buffwriter_printf();
    return EXIT_SUCCESS;
}

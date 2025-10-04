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
#include "buffprinter.h"


void
test_printer() {
    char buff[16];
    struct buffprinter w = {buff, sizeof(buff), 0};

    eqint(8, buffprint(&w, "foo %s", "bar"));
    eqint(7, w.used);

    eqint(4, buffprint(&w, " baz"));
    eqint(11, w.used);

    eqint(-1, buffprint(&w, " thud "));
    eqint(11, w.used);
}


int
main() {
    test_printer();
    return EXIT_SUCCESS;
}

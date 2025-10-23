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

/* local private */
#include "fixtures.h"


static void
test_response_startline() {
    struct chttp_response *r = chttp_response_new(3);
    isnotnull(r);

    eqint(200, responsef(r, "HTTP/1.1 200 OK\r\n"));
    eqint(200, r->status);
    eqstr("OK", r->text);
    eqstr("HTTP/1.1", r->protocol);

    chttp_response_free(r);
}


int
main() {
    test_response_startline();
    return EXIT_SUCCESS;
}


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
#include <string.h>

/* thirdparty */
#include <cutest.h>

/* local public */
#include "chttp.h"


void
test_response() {
    struct chttp_response resp;
    char buff[1024];
    int bufflen = 1024;
    int contentlen;

    chttp_response_start(&resp, 200, "Ok");
    contentlen = chttp_response_tobuff(&resp, buff, &bufflen);
    eqint(17, contentlen);
    eqstr("HTTP/1.1 200 Ok\r\n", buff);

    // chttp_response_header(&resp, "foo = %s", "bar");
}


int
main() {
    test_response();
    return EXIT_SUCCESS;
}

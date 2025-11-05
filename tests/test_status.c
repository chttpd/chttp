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
#include "chttp/chttp.h"


void
test_status_text() {
    int i;

    for (i = 0; i < 200; i++) {
        isnull(chttp_status_text(i));
    }
    eqstr("Ok", chttp_status_text(CHTTP_STATUS_200_OK));
    eqstr("Bad Request", chttp_status_text(CHTTP_STATUS_400_BADREQUEST));
    eqstr("Not Found", chttp_status_text(CHTTP_STATUS_404_NOTFOUND));
    eqstr("Internal Server Error",
            chttp_status_text(CHTTP_STATUS_500_INTERNALSERVERERROR));
    for (i = 600; i < 1000; i++) {
        isnull(chttp_status_text(i));
    }
}


int
main() {
    test_status_text();
    return EXIT_SUCCESS;
}

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
#include <stdarg.h>
#include <stdio.h>


/* local private */
#include "fixtures.h"


static char _reqbuff[CONFIG_SYSTEM_PAGESIZE * 4];


chttp_status_t
requestf(struct chttp_request *r, const char *fmt, ...) {
    size_t bytes;
    va_list args;

    va_start(args, fmt);
    bytes = vsprintf(_reqbuff, fmt, args);
    va_end(args);

    if (bytes < 0) {
        return -1;
    }

    return chttp_request_parse(r, _reqbuff, bytes);
}

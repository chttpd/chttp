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
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/* local private */
#include "str.h"
#include "uri.h"
#include "request.h"


static int
_startline_parse(struct chttp_request *req, char *line) {
    char *uri;

    if (strsplit(line, " ",
            (char **[]){&req->verb, &uri, &req->protocol}, 3) != 3) {
        return -1;
    }

    /* querystring */
    if (strsplit(uri, "?", (char **[]){&req->path, &req->query}, 2) == -1) {
        return -1;
    }

    uridecode(req->query);
    return 0;
}


httpstatus_t
request_frombuffer(struct chttp_request *req, char *header) {
    char *line;
    char *saveptr;

    /* startline */
    line = strtok_r(header, "\n", &saveptr);
    if (line == NULL) {
        return 414;
    }

    if (_startline_parse(req, line)) {
        return 414;
    }

    return 0;
}


httpstatus_t
request_fromstring(struct chttp_request *req, const char *fmt, ...) {
    int ret;
    char tmp[CONFIG_CHTTP_REQUEST_BUFFSIZE];

    va_list args;

    va_start(args, fmt);
    ret = vsprintf(tmp, fmt, args);
    va_end(args);

    if (ret < 0) {
        return -1;
    }

    return request_frombuffer(req, tmp);
}

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
#include "linearbuffer.h"
#include "request.h"


static int
_startline_parse(struct chttp_request *req, char *line) {
    int ret;
    char *url;
    char **tokens[3] = {
        &req->verb,
        &url,
        &req->protocol
    };

    if (strsplit(line, " ", 3, tokens) != 3) {
        return -1;
    }
    req->verb = linearbuffer_allocate(&req->buff, req->verb);
    req->protocol = linearbuffer_allocate(&req->buff, req->protocol);

    /* querystring */
    tokens[0] = &req->path;
    tokens[1] = &req->query;
    ret = strsplit(url, "?", 2, tokens);
    if (ret < 1) {
        return -1;
    }

    req->path = linearbuffer_allocate(&req->buff, req->path);
    if (ret == 2) {
        uridecode(req->query);
        req->query = linearbuffer_allocate(&req->buff, req->query);
    }
    else {
        req->query = NULL;
    }

    return 0;
}


httpstatus_t
request_frombuffer(struct chttp_request *req, char *header, size_t size) {
    int count = 0;
    char *line;
    char *saveptr;

    if (size < 16) {
        return 400;
    }

    if (size > CONFIG_CHTTP_REQUEST_BUFFSIZE) {
        return 431;
    }

    if (strncmp("\n\n", header + (size - 2), 2) == 0) {
        size--;
    }
    else if (strncmp("\r\n\r\n", header + (size - 4), 4) == 0) {
        size -= 2;
    }
    else {
        return 400;
    }

    header[size] = 0;

    memset(req, 0, sizeof(struct chttp_request));
    linearbuffer_init(&req->buff, req->sharedbuff,
            CHTTP_REQUEST_SHAREDBUFF_SIZE);

    /* startline */
    line = strtok_r(header, "\n", &saveptr);
    if (line == NULL) {
        return 414;
    }

    if (_startline_parse(req, line)) {
        return 414;
    }

    count = linearbuffer_splitallocate(&req->buff, saveptr, "\n",
            req->headers, CONFIG_CHTTP_REQUEST_HEADERSMAX);
    if (count == -1) {
        /* extra token found */
        return 431;
    }
    if (count == -2) {
        /* zero length heder found */
        return 400;
    }

    req->headerscount = count;
    return 0;
}


httpstatus_t
request_fromstring(struct chttp_request *req, const char *fmt, ...) {
    size_t bytes;
    va_list args;
    char tmp[CONFIG_CHTTP_REQUEST_BUFFSIZE];

    va_start(args, fmt);
    bytes = vsprintf(tmp, fmt, args);
    va_end(args);

    if (bytes < 0) {
        return -1;
    }

    return request_frombuffer(req, tmp, bytes);
}

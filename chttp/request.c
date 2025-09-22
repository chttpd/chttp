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
#include "store.h"
#include "request.h"


static int
_startline_parse(struct chttp_request *req, char *line) {
    char **tokens[4] = {&req->verb, &req->path, &req->protocol, &req->query};

    /* verb/uri/protocol */
    if (strtokenize(line, " ", 3, tokens) != 3) {
        return -1;
    }

    /* path/querystring */
    switch (strtokenize(req->path, "?", 2, (char **[]){&req->path,
                &req->query})) {
        case 2:
            uridecode(req->query);
            break;
        case 1:
            req->query = NULL;
            break;
        default:
            return -1;
    }

    return store_replaceall(&req->buff, 4, tokens);
}


static httpstatus_t
_headers_parse(struct chttp_request *req, char *headers) {
    int i;
    int count;
    char **ptrs[CONFIG_CHTTP_REQUEST_HEADERSMAX];

    for (i = 0; i < CONFIG_CHTTP_REQUEST_HEADERSMAX; i++) {
        ptrs[i] = &req->headers[i];
    }

    count = strtokenize(headers, "\n", CONFIG_CHTTP_REQUEST_HEADERSMAX, ptrs);
    if (count == -1) {
        /* extra token found */
        return 400;
    }

    if (count == -2) {
        /* zero length heder found */
        return 431;
    }

    if (count == 0) {
        return 0;
    }

    if (store_replaceall(&req->buff, count, ptrs)) {
        return 500;
    }

    req->headerscount = count;
    return 0;
}


httpstatus_t
request_frombuffer(struct chttp_request *req, char *header, size_t size) {
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
    store_init(&req->buff, req->sharedbuff,
            CHTTP_REQUEST_SHAREDBUFF_SIZE);

    /* startline */
    line = strtok_r(header, "\n", &saveptr);
    if (line == NULL) {
        return 414;
    }

    if (_startline_parse(req, line)) {
        return 414;
    }

    return _headers_parse(req, saveptr);
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

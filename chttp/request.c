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
_contenttype_parse(struct chttp_request *req, char *in) {
    char *tokens[2];

    switch (strtokenize(in, ";", 2, tokens)) {
        case 2:
            if (strcasestr(tokens[1], "charset=") == tokens[1]) {
                tokens[1] += 8;
            }
            break;
        case 1:
            break;
        default:
            return -1;
    }

    return store_all(&req->store, 2, (const char **[]) {
            &req->contenttype, &req->charset}, (const char **)tokens);
    return 0;
}


static int
_pathquery_split(char *uri, char **path, char **query) {
    char *tokens[2];

    switch (strtokenize(uri, "?", 2, tokens)) {
        case 2:
            uridecode(tokens[1]);
            *query = tokens[1];
            break;
        case 1:
            *query = NULL;
            break;
        default:
            return -1;
    }

    *path = tokens[0];
    return 0;
}


static int
_startline_parse(struct chttp_request *req, char *line) {
    char *tokens[4];

    /* verb/uri/protocol */
    if (strtokenize(line, " ", 3, tokens) != 3) {
        return -1;
    }

    if (_pathquery_split(tokens[1], &tokens[1], &tokens[3])) {
        return -1;
    }

    return store_all(&req->store, 4, (const char **[]) {
            &req->verb, &req->path, &req->protocol, &req->query
        }, (const char **)tokens);
}


static int
_header_known(struct chttp_request *req, char *header) {
    char *tmp;
    int ret;

    if (strcasestr(header, "content-length:") == header) {
        req->contentlength = atoi(strtrim(header + 15, NULL));
        return 1;
    }

    ret = store_ifci(&req->store, &req->useragent, header, "user-agent:");
    if (ret) {
        return ret;
    }

    ret = store_ifci(&req->store, &req->expect, header, "expect:");
    if (ret) {
        return ret;
    }

    if (strcasestr(header, "content-type:") == header) {
        tmp = strtrim(header + 13, NULL);
        _contenttype_parse(req, tmp);
        return 1;
    }

    if (strcasestr(header, "connection:") == header) {
        tmp = strtrim(header + 11, NULL);
        if (strcasecmp("close", tmp) == 0) {
            req->connection = CHTTP_CONNECTION_CLOSE;
        }
        else if (strcasecmp("keep-alive", tmp) == 0) {
            req->connection = CHTTP_CONNECTION_KEEPALIVE;
        }
        else {
            return -1;
        }
        return 1;
    }

    return 0;
}


static chttp_status_t
_headers_parse(struct chttp_request *req, char *headers) {
    int ret;
    int i;
    int count = 0;
    char *token;
    char *saveptr = NULL;
    char *hdrs[CONFIG_CHTTP_REQUEST_HEADERSMAX];
    const char **ptrs[CONFIG_CHTTP_REQUEST_HEADERSMAX];

    for (i = 0; i < CONFIG_CHTTP_REQUEST_HEADERSMAX; i++) {
        token = strtoktrim(i? NULL: headers, "\n", &saveptr);
        if (token == NULL) {
            break;
        }

        if (!token[0]) {
            /* zero length header found */
            return 431;
        }

        ret = _header_known(req, token);
        if (ret == -1) {
            return 400;
        }

        if (ret > 0) {
            continue;
        }

        hdrs[count++] = token;
    }

    if (count == 0) {
        return 0;
    }

    if (saveptr[0]) {
        return 400;
    }

    for (i = 0; i < count; i++) {
        ptrs[i] = &req->headers[i];
    }

    /* apply the store functor to all pointers */
    if (store_all(&req->store, count, ptrs, (const char **)hdrs)) {
        return 500;
    }

    req->headerscount = count;
    return 0;
}


chttp_status_t
chttp_request_frombuffer(struct chttp_request *req, char *header,
        size_t size) {
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
    store_init(&req->store, req->storebuff, CHTTP_REQUEST_STORE_BUFFSIZE);

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


chttp_status_t
chttp_request_fromstring(struct chttp_request *req, const char *fmt, ...) {
    size_t bytes;
    va_list args;
    char tmp[CONFIG_CHTTP_REQUEST_BUFFSIZE];

    va_start(args, fmt);
    bytes = vsprintf(tmp, fmt, args);
    va_end(args);

    if (bytes < 0) {
        return -1;
    }

    return chttp_request_frombuffer(req, tmp, bytes);
}

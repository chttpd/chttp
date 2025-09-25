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
#include <stdarg.h>
#include <string.h>

/* local private */
#include "store.h"
#include "response.h"

/* local public */
#include "chttp.h"


static const char *_proto = "HTTP/1.1";


int
chttp_response_start(struct chttp_response *resp, int status, char *text) {
    memset(resp, 0, sizeof(struct chttp_response));
    store_init(&resp->store, resp->storebuff, CHTTP_RESPONSE_STORE_BUFFSIZE);
    resp->text = store_allocate(&resp->store, text);
    if (resp->text == NULL) {
        return -1;
    }

    resp->protocol = _proto;
    resp->status = status;
    return 0;
}


int
chttp_response_tobuff(struct chttp_response *resp, char *buff, int *len) {
    int i;
    int bytes = 0;

    bytes += sprintf(buff, "%s %d %s\r\n", resp->protocol, resp->status,
            resp->text);

    for (i = 0; i < resp->headerscount; i++) {
        bytes += sprintf(buff + bytes, "%s\r\n", resp->headers[i]);
    }

    bytes += sprintf(buff + bytes, "\r\n");
    return bytes;
}


int
chttp_response_header(struct chttp_response *resp, const char *fmt, ...) {
    va_list args;
    const char *h;
    // TODO: config
    char buff[1024];

    va_start(args, fmt);
    vsnprintf(buff, sizeof(buff), fmt, args);
    va_end(args);

    h = store_allocate(&resp->store, buff);
    resp->headers[resp->headerscount++] = h;
    return 0;
}

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
chttp_response_start(struct chttp_response *resp, chttp_status_t status,
        const char *text) {
    const char *txt = text;
    memset(resp, 0, sizeof(struct chttp_response));
    resp->contentlength = -1;

    store_init(&resp->store, resp->storebuff, CHTTP_RESPONSE_STORE_BUFFSIZE);

    if (txt == NULL) {
        txt = chttp_status_text(status);
    }

    if (txt == NULL) {
        return -1;
    }

    resp->text = store_one(&resp->store, txt);
    if (resp->text == NULL) {
        return -1;
    }

    resp->protocol = _proto;
    resp->status = status;
    return 0;
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

    h = store_one(&resp->store, buff);
    resp->headers[resp->headerscount++] = h;
    return 0;
}


int
chttp_response_contenttype(struct chttp_response *resp, const char *type,
        const char *charset) {
    const char *ct;
    const char *cs;

    ct = store_one(&resp->store, type);
    if (ct == NULL) {
        return -1;
    }

    resp->contenttype = ct;
    if (charset) {
        cs = store_one(&resp->store, charset);
        if (cs == NULL) {
            return -1;
        }
        resp->charset = cs;
    }

    return 0;
}


int
chttp_response_write(struct chttp_response *resp, const char *fmt, ...) {
    va_list args;
    int bytes;
    size_t maxlen;

    if (resp->content == NULL) {
        resp->contentmax = store_avail(&resp->store);
        resp->content = store_allocate(&resp->store, resp->contentmax);
        resp->contentlength = 0;
    }

    maxlen = resp->contentmax - resp->contentlength;

    va_start(args, fmt);
    bytes = vsnprintf(resp->content + resp->contentlength, maxlen, fmt, args);
    va_end(args);

    resp->contentlength += bytes;
    return bytes;
}


int
chttp_response_tobuff(struct chttp_response *resp, char *buff, int *len) {
    int i;
    int bytes = 0;

    bytes += sprintf(buff, "%s %d %s\r\n", resp->protocol, resp->status,
            resp->text);

    /* known headers */
    if (resp->contentlength > -1) {
        bytes += sprintf(buff + bytes, "Content-Length: %ld\r\n",
                resp->contentlength);
    }

    /* content/mime type */
    if (resp->contenttype) {
        bytes += sprintf(buff + bytes, "Content-Type: %s", resp->contenttype);
        if (resp->charset) {
            bytes += sprintf(buff + bytes, "; charset=%s", resp->charset);
        }
        bytes += sprintf(buff + bytes, "\r\n");
    }

    /* headers */
    for (i = 0; i < resp->headerscount; i++) {
        bytes += sprintf(buff + bytes, "%s\r\n", resp->headers[i]);
    }

    /* close the http head */
    bytes += sprintf(buff + bytes, "\r\n");

    /* content */
    if (resp->contentlength > 0) {
        if (resp->content == NULL) {
            return -1;
        }
        strncpy(buff + bytes, resp->content, resp->contentlength);
        bytes += resp->contentlength;
    }

    return bytes;
}

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
#include "responsemaker.h"

/* local public */
#include "chttp.h"


static const char *_proto = "HTTP/1.1";


int
chttp_responsemaker_start(struct chttp_request *r, chttp_status_t status,
        const char *text) {
    struct chttp_responsemaker *resp = &r->response;
    size_t len;

    if (text == NULL) {
        text = chttp_status_text(status);
    }

    if (text == NULL) {
        return -1;
    }

    if (store_strf(&r->store, &resp->header, &len, "%s %d %s\r\n", _proto,
                status, text)) {
        return -1;
    }

    resp->headerlen += len;
    return 0;
}


int
chttp_responsemaker_vheader(struct chttp_request *r, const char *fmt,
        va_list args) {
    struct chttp_responsemaker *resp = &r->response;
    int ret;
    size_t len;

    if (fmt == NULL) {
        return -1;
    }

    ret = store_vappendf(&r->store, &len, fmt, args);

    if (ret) {
        return -1;
    }

    resp->headerlen += len;
    if (store_append(&r->store, &len, "\r\n")) {
        return -1;
    }

    resp->headerlen += len;
    return 0;
}


int
chttp_responsemaker_header(struct chttp_request *r, const char *fmt, ...) {
    va_list args;
    int ret;

    va_start(args, fmt);
    ret = chttp_responsemaker_vheader(r, fmt, args);
    va_end(args);

    return ret;
}


int
chttp_responsemaker_contenttype(struct chttp_request *r, const char *type,
        const char *charset) {
    if (type == NULL) {
        return -1;
    }

    if (charset) {
        if (chttp_responsemaker_header(r, "Content-Type: %s; charset=%s",
                    type, charset)) {
            return -1;
        }

        return 0;
    }

    if (chttp_responsemaker_header(r, "Content-Type: %s", type)) {
        return -1;
    }

    return 0;
}


int
chttp_responsemaker_header_close(struct chttp_request *r) {
    struct chttp_responsemaker *resp = &r->response;

    if (resp->content) {
        if (chttp_responsemaker_header(r, "Content-Length: %d",
                    resp->contentlength)) {
            return -1;
        }
    }

    if (store_append(&r->store, NULL, "\r\n")) {
        return -1;
    }

    resp->headerlen += 2;
    return 0;
}


int
chttp_responsemaker_content_allocate(struct chttp_request *r, size_t size) {
    struct chttp_responsemaker *resp = &r->response;

    if (resp->content) {
        return -1;
    }

    resp->content = malloc(size);
    if (resp->content == NULL) {
        return -1;
    }

    resp->contentmax = size;
    resp->contentlength = 0;
    return 0;
}


ssize_t
chttp_responsemaker_content_vwrite(struct chttp_request *r, const char *fmt,
        va_list args) {
    struct chttp_responsemaker *resp = &r->response;
    ssize_t bytes;
    size_t avail;

    if (resp->content == NULL) {
        return -1;
    }

    avail = resp->contentmax - resp->contentlength;
    bytes = vsnprintf(resp->content + resp->contentlength, avail, fmt, args);

    if (bytes >= avail) {
        /* output truncated */
        return -1;
    }

    resp->contentlength += bytes;
    return bytes;
}


ssize_t
chttp_responsemaker_content_write(struct chttp_request *r, const char *fmt,
        ...) {
    va_list args;
    ssize_t bytes;

    va_start(args, fmt);
    bytes = chttp_responsemaker_content_vwrite(r, fmt, args);
    va_end(args);

    return bytes;
}

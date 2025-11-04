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
#include <unistd.h>

/* local private */
#include "store.h"
#include "responsebuilder.h"

/* local public */
#include "chttp.h"


static const char *_proto = "HTTP/1.1";


int
chttp_responsebuilder_allocate(struct chttp_responsebuilder *r,
        int headerpages, int contentpages) {
    void *h;
    void *c = NULL;
    size_t pagesize = getpagesize();

    h = malloc(pagesize * headerpages);
    if (h == NULL) {
        return -1;
    }

    if (contentpages > 0) {
        c = malloc(pagesize * contentpages);
        if (c == NULL) {
            free(h);
            return -1;
        }
    }

    r->header = h;
    r->headermax = pagesize * headerpages;
    r->headerlen = 0;

    if (c) {
        r->contentmax = pagesize * contentpages;
    }
    r->content = c;
    r->contentlen = 0;

    return 0;
}


void
chttp_responsebuilder_free(struct chttp_responsebuilder *r) {
    free(r->header);

    if (r->content) {
        free(r->content);
    }
}


void
chttp_responsebuilder_reset(struct chttp_responsebuilder *r) {
    r->headerlen = 0;
    r->contentlen = 0;
}


int
chttp_responsebuilder_start(struct chttp_responsebuilder *r,
        chttp_status_t status, const char *text) {
    size_t len;

    if (status <= 0) {
        return -1;
    }

    if (text == NULL) {
        text = chttp_status_text(status);
    }

    if (text == NULL) {
        return -1;
    }


    len = snprintf(r->header, r->headermax, "%s %d %s\r\n", _proto, status,
            text);
    if (len >= r->headermax) {
        return -1;
    }

    r->headerlen += len;
    return 0;
}


static int
_vhprintf(struct chttp_responsebuilder *r,
        const char *fmt, va_list args) {
    size_t len;
    size_t avail;
    char *p;

    avail = r->headermax - r->headerlen;
    p = r->header + r->headerlen;
    len = vsnprintf(p, avail, fmt, args);
    if (len >= avail) {
        return -1;
    }

    r->headerlen += len;
    return 0;
}


static int
_hprintf(struct chttp_responsebuilder *r,
        const char *fmt, ...) {
    va_list args;
    int ret;

    va_start(args, fmt);
    ret = _vhprintf(r, fmt, args);
    va_end(args);

    return ret;
}


int
chttp_responsebuilder_vheader(struct chttp_responsebuilder *r,
        const char *fmt, va_list args) {
    if (_vhprintf(r, fmt, args)) {
        return -1;
    }

    if (_hprintf(r, "\r\n")) {
        return -1;
    }

    return 0;
}


int
chttp_responsebuilder_header(struct chttp_responsebuilder *r,
        const char *fmt, ...) {
    va_list args;
    int ret;

    va_start(args, fmt);
    ret = chttp_responsebuilder_vheader(r, fmt, args);
    va_end(args);

    return ret;
}


int
chttp_responsebuilder_contenttype(struct chttp_responsebuilder *r,
        const char *type, const char *charset) {
    if (type == NULL) {
        return -1;
    }

    if (charset) {
        if (_hprintf(r, "Content-Type: %s; charset=%s\r\n", type, charset)) {
            return -1;
        }

        return 0;
    }

    if (_hprintf(r, "Content-Type: %s\r\n", type)) {
        return -1;
    }

    return 0;
}


int
chttp_responsebuilder_close(struct chttp_responsebuilder *r) {
    if (r->content) {
        if (_hprintf(r, "Content-Length: %d\r\n\r\n", r->contentlen)) {
            return -1;
        }

        return 0;
    }

    if (_hprintf(r, "\r\n")) {
        return -1;
    }

    return 0;
}


int
chttp_responsebuilder_vwrite(struct chttp_responsebuilder *r,
        const char *fmt, va_list args) {
    size_t len;
    size_t avail;
    char *p;

    avail = r->contentmax - r->contentlen;
    p = r->content + r->contentlen;
    len = vsnprintf(p, avail, fmt, args);
    if (len >= avail) {
        return -1;
    }

    r->contentlen += len;
    return 0;
}


int
chttp_responsebuilder_write(struct chttp_responsebuilder *r, const char *fmt,
        ...) {
    va_list args;
    ssize_t bytes;

    va_start(args, fmt);
    bytes = chttp_responsebuilder_vwrite(r, fmt, args);
    va_end(args);

    return bytes;
}

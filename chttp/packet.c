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

/* system */
#include <sys/uio.h>

/* local public */
#include "chttp.h"

/* local private */
#include "common.h"
#include "str.h"


static const char *_proto = "HTTP/1.1";


int
chttp_packet_allocate(struct chttp_packet *p, int headerpages,
        int contentpages, int encoding) {
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

    p->header = h;
    p->headermax = pagesize * headerpages;
    p->headerlen = 0;

    if (c) {
        p->contentmax = pagesize * contentpages;
        p->encoding = encoding;
    }
    p->content = c;
    p->contentlen = 0;

    return 0;
}


void
chttp_packet_free(struct chttp_packet *p) {
    free(p->header);

    if (p->content) {
        free(p->content);
    }
}


void
chttp_packet_reset(struct chttp_packet *p) {
    p->headerlen = 0;
    p->contentlen = 0;
}


int
chttp_packet_startresponse(struct chttp_packet *p, chttp_status_t status,
        const char *text) {
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


    len = snprintf(p->header, p->headermax, "%s %d %s\r\n", _proto, status,
            text);
    if (len >= p->headermax) {
        return -1;
    }

    p->headerlen += len;
    return 0;
}


static int
_vhprintf(struct chttp_packet *p, const char *fmt, va_list args) {
    ssize_t len;
    size_t avail;
    char *s;

    avail = p->headermax - p->headerlen;
    s = p->header + p->headerlen;
    len = vsnprintf(s, avail, fmt, args);
    if ((len < 0) || (len >= avail)) {
        return -1;
    }

    p->headerlen += len;
    return 0;
}


static int
_hprintf(struct chttp_packet *p, const char *fmt, ...) {
    va_list args;
    int ret;

    va_start(args, fmt);
    ret = _vhprintf(p, fmt, args);
    va_end(args);

    return ret;
}


int
chttp_packet_vheader(struct chttp_packet *p, const char *fmt,
        va_list args) {
    ERR(_vhprintf(p, fmt, args));
    ERR(_hprintf(p, "\r\n"));
    return 0;
}


int
chttp_packet_header(struct chttp_packet *p, const char *fmt, ...) {
    va_list args;
    int ret;

    va_start(args, fmt);
    ret = chttp_packet_vheader(p, fmt, args);
    va_end(args);

    return ret;
}


int
chttp_packet_contenttype(struct chttp_packet *p, const char *type,
        const char *charset) {
    if (type == NULL) {
        return -1;
    }

    if (charset) {
        if (_hprintf(p, "Content-Type: %s; charset=%s\r\n", type, charset)) {
            return -1;
        }

        return 0;
    }

    if (_hprintf(p, "Content-Type: %s\r\n", type)) {
        return -1;
    }

    return 0;
}


int
chttp_packet_transferencoding(struct chttp_packet *p, int encoding) {
    ERR(_hprintf(p, "Transfer-Encoding: "));

    if (encoding & CHTTP_TE_CHUNKED) {
        ERR(_hprintf(p, "chunked"));
    }

    ERR(_hprintf(p, "\r\n"));
    p->encoding = encoding;
    return 0;
}


int
chttp_packet_close(struct chttp_packet *p) {
    if (p->content) {
        if (_hprintf(p, "Content-Length: %d\r\n\r\n", p->contentlen)) {
            return -1;
        }

        return 0;
    }

    if (_hprintf(p, "\r\n")) {
        return -1;
    }

    return 0;
}


int
chttp_packet_vwrite(struct chttp_packet *p, const char *fmt,
        va_list args) {
    size_t len;
    size_t avail;
    char *s;

    avail = p->contentmax - p->contentlen;
    s = p->content + p->contentlen;
    len = vsnprintf(s, avail, fmt, args);
    if (len >= avail) {
        return -1;
    }

    p->contentlen += len;
    return 0;
}


int
chttp_packet_write(struct chttp_packet *p, const char *fmt, ...) {
    va_list args;
    ssize_t bytes;

    va_start(args, fmt);
    bytes = chttp_packet_vwrite(p, fmt, args);
    va_end(args);

    return bytes;
}


ssize_t
chttp_packet_iovec(struct chttp_packet *p, struct iovec v[], int *vcount) {
    int count = 0;
    int vmax = *vcount;
    size_t totallen = 0;
    ssize_t clen;
    int ccount;

    if (p->headerlen) {
        ASSRT(count < vmax);
        v[count].iov_base = (void *)p->header;
        v[count].iov_len = p->headerlen;
        count++;
        totallen = p->headerlen;
    }

    if (p->content) {
        if (p->encoding == CHTTP_TE_CHUNKED) {
            ccount = vmax - count;
            clen = chttp_chunked_iovec(p->content, p->contentlen, v + count,
                    &ccount);
            ASSRT(clen > 0);
            count += ccount;
            totallen += clen;
        }
        else {
            ASSRT(count < vmax);
            v[count].iov_base = p->content;
            v[count].iov_len = p->contentlen;
            count++;
            totallen += p->contentlen;
        }
    }

    if (count == 0) {
        return -1;
    }

    *vcount = count;
    return totallen;
}

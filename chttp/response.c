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
#include <string.h>
#include <unistd.h>

/* local private */
#include "common.h"
#include "store.h"
#include "str.h"

/* local public */
#include "chttp/chttp.h"


/** determine and store known headers.
  * return:
  * -1 error
  *  1 unknown header
  *  0 known header
  */
static int
_header_known(struct chttp_response *r, char *header) {
    char *tmp;
    int ret;

    if (strcasestr(header, "transfer-encoding:") == header) {
        ret = transferencoding(chttp_str_trim(header + 18, NULL));
        if (ret == -1) {
            return -1;
        }

        r->transferencoding = ret;
        return 0;
    }

    if (strcasestr(header, "content-length:") == header) {
        r->contentlength = atoi(chttp_str_trim(header + 15, NULL));
        return 0;
    }

    if (str_startswith_ci(header, "content-type:")) {
        tmp = chttp_str_trim(header + 13, NULL);
        if (contenttype_parse(&r->store, tmp, &r->contenttype, &r->charset)) {
            return -1;
        }

        return 0;
    }

    return 1;
}


static chttp_status_t
_headers_parse(struct chttp_response *r, char *headers) {
    int ret;
    int i;
    int count = 0;
    char *token;
    char *saveptr = NULL;
    char *hdrs[CONFIG_CHTTP_HEADERSMAX];
    const char **ptrs[CONFIG_CHTTP_HEADERSMAX];

    for (i = 0; i < CONFIG_CHTTP_HEADERSMAX; i++) {
        token = str_tokenize(i? NULL: headers, "\r", &saveptr);
        if (token == NULL) {
            break;
        }

        if (!token[0]) {
            /* zero length header found */
            return -1;
        }

        ret = _header_known(r, token);
        if (ret == -1) {
            return -1;
        }

        if (ret == 0) {
            continue;
        }

        hdrs[count++] = token;
    }

    if (count == 0) {
        return 0;
    }

    if (saveptr[0]) {
        return -1;
    }

    for (i = 0; i < count; i++) {
        ptrs[i] = &r->headers.list[i];
    }

    /* apply the store functor to all pointers */
    if (count != store_all(&r->store, count, ptrs, (const char **)hdrs)) {
        return -1;
    }

    r->headers.count = count;
    return 0;
}


static int
_startline_parse(struct chttp_response *r, char *line) {
    char *saveptr;
    char *status;

    /* protocol/status/text */
    r->protocol = str_tokenize(line, " ", &saveptr);
    status = str_tokenize(NULL, " ", &saveptr);

    if ((saveptr[0] == '\r') || (!saveptr[0])) {
        return -1;
    }

    r->status = atoi(status);
    r->text = saveptr;

    if (2 != store_all(&r->store, 2,
                (const char **[]) {&r->protocol, &r->text},
                (const char *[]){r->protocol, r->text})) {
        return -1;
    }

    return 0;
}


struct chttp_response *
chttp_response_new(uint8_t pages) {
    struct chttp_response *r;
    int pagesize = getpagesize();
    size_t total = pages * pagesize;

    r = malloc(total);
    if (r == NULL) {
        return NULL;
    }

    store_init(&r->store, r->storebuff, total - sizeof(struct chttp_response));

    /* zero fill */
    memset(r, 0, ((void *)&r->store) - ((void *)r));
    r->contentlength = -1;
    return r;
}


void
chttp_response_reset(struct chttp_response *r) {
    memset(r, 0, ((void *)&r->store) - ((void *)r));
    r->store.len = 0;
    r->contentlength = -1;
}


chttp_status_t
chttp_response_parse(struct chttp_response *r, char *header, size_t size) {
    char *line;
    char *saveptr;

    if (size < 16) {
        return -1;
    }

    if (size > store_avail(r->store)) {
        return -1;
    }

    /* null termination for strtok_r */
    header[size - 1] = 0;

    /* startline */
    line = strtok_r(header, "\r\n", &saveptr);
    if (line == NULL) {
        return -1;
    }

    if (_startline_parse(r, line)) {
        return -1;
    }

    if (saveptr && (saveptr[0] == '\n')) {
        saveptr++;
    }

    return _headers_parse(r, saveptr);
}


int
chttp_response_free(struct chttp_response *r) {
    if (r == NULL) {
        return -1;
    }

    free(r);
    return 0;
}

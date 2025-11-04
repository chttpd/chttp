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
#include "chttp.h"


/** determine and store known headers.
  * return:
  * -1 error
  *  1 unknown header
  *  0 known header
  */
static int
_header_known(struct chttp_request *r, char *header) {
    char *tmp;
    int ret;

    if (strcasestr(header, "transfer-encoding:") == header) {
        ret = chttp_transferencoding(str_trim(header + 18, NULL));
        if (ret == -1) {
            return -1;
        }

        r->transferencoding = ret;
        return 0;
    }

    if (strcasestr(header, "content-length:") == header) {
        r->contentlength = atoi(str_trim(header + 15, NULL));
        return 0;
    }

    ret = store_suffixifprefix_ci(&r->store, &r->useragent, NULL, header,
            "user-agent:");
    if (ret <= 0) {
        return ret;
    }

    ret = store_suffixifprefix_ci(&r->store, &r->expect, NULL, header,
            "expect:");
    if (ret <= 0) {
        return ret;
    }

    if (str_startswith_ci(header, "content-type:")) {
        tmp = str_trim(header + 13, NULL);
        if (chttp_contenttype_parse(&r->store, tmp, &r->contenttype,
                    &r->charset)) {
            return -1;
        }

        return 0;
    }

    return 1;
}


static chttp_status_t
_headers_parse(struct chttp_request *r, char *headers) {
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
            return CHTTP_STATUS_400_BADREQUEST;
        }

        ret = _header_known(r, token);
        if (ret == -1) {
            return CHTTP_STATUS_400_BADREQUEST;
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
        return CHTTP_STATUS_400_BADREQUEST;
    }

    for (i = 0; i < count; i++) {
        ptrs[i] = &r->headers.list[i];
    }

    /* apply the store functor to all pointers */
    if (count != store_all(&r->store, count, ptrs, (const char **)hdrs)) {
        return CHTTP_STATUS_500_INTERNALSERVERERROR;
    }

    r->headers.count = count;
    return 0;
}


static int
_pathquery_split(char *uri, char **path, char **query) {
    char *tokens[2];

    switch (str_tokenizeall(uri, "?", 2, tokens)) {
        case 2:
            chttp_uridecode(tokens[1]);
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
_startline_parse(struct chttp_request *r, char *line) {
    char *tokens[4];

    /* verb/uri/protocol */
    if (str_tokenizeall(line, " ", 3, tokens) != 3) {
        return -1;
    }

    if (_pathquery_split(tokens[1], &tokens[1], &tokens[3])) {
        return -1;
    }

    if (4 != store_all(&r->store, 4, (const char **[]) {
                &r->verb, &r->path, &r->protocol, &r->query
            }, (const char **)tokens)) {
        return -1;
    }

    return 0;
}


struct chttp_request *
chttp_request_new(int pages) {
    struct chttp_request *r;
    int pagesize = getpagesize();
    size_t total = pages * pagesize;

    r = malloc(total);
    if (r == NULL) {
        return NULL;
    }

    store_init(&r->store, r->storebuff, total - sizeof(struct chttp_request));

    /* zero fill */
    memset(r, 0, ((void *)&r->store) - ((void *)r));
    r->contentlength = -1;
    return r;
}


chttp_status_t
chttp_request_parse(struct chttp_request *r, char *header, size_t size) {
    char *line;
    char *saveptr;

    if (size < 16) {
        return CHTTP_STATUS_400_BADREQUEST;
    }

    if (size > store_avail(r->store)) {
        return CHTTP_STATUS_431_REQUESTHEADERFIELDSTOOLARGE;
    }

    /* null termination for strtok_r */
    header[size - 1] = 0;

    /* startline */
    line = strtok_r(header, "\r\n", &saveptr);
    if (line == NULL) {
        return CHTTP_STATUS_414_URITOOLONG;
    }

    if (_startline_parse(r, line)) {
        return CHTTP_STATUS_400_BADREQUEST;
    }

    if (saveptr && (saveptr[0] == '\n')) {
        saveptr++;
    }

    return _headers_parse(r, saveptr);
}


void
chttp_request_reset(struct chttp_request *r) {
    memset(r, 0, ((void *)&r->store) - ((void *)r));
    r->store.len = 0;
    r->contentlength = -1;
}

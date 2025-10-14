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

/* local private */
#include "chttp.h"
#include "store.h"

/* local private */
#include "request.h"


struct chttp_request *
chttp_request_new(uint8_t pages) {
    struct chttp_request *r;
    size_t total = pages * CONFIG_SYSTEM_PAGESIZE;

    r = malloc(total);
    if (r == NULL) {
        return NULL;
    }

    memset(r, 0, sizeof(struct chttp_request));
    r->contentlength = -1;
    store_init(&r->store, r->storebuff, total - sizeof(struct chttp_request));

    return r;
}


chttp_status_t
chttp_request_parse(struct chttp_request *r, char *header, size_t size) {
    char *line;
    char *saveptr;

    if (size < 16) {
        return 400;
    }

    if (size > store_avail(r->store)) {
        return 431;
    }

    /* null termination for strtok_r */
    header[size - 1] = 0;

    /* reset the request */
    memset(r, 0, ((void *)&r->store) - ((void *)r));
    r->contentlength = -1;
    r->store.len = 0;

    /* startline */
    line = strtok_r(header, "\r", &saveptr);
    if (line == NULL) {
        return 414;
    }

    return 0;
    // if (_startline_parse(r, line)) {
    //     return 414;
    // }

    // return _headers_parse(r, saveptr);
}



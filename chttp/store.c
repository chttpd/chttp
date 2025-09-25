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
#include "str.h"
#include "store.h"


void
store_init(struct chttp_store *lb, char *backend, size_t size) {
    lb->backend = backend;
    // TODO: delete size or check it on allocation
    lb->size = size;
    lb->len = 0;
}


const char *
store_allocate(struct chttp_store *lb, const char *str) {
    char *start;
    char *end;
    int len;

    if (str == NULL) {
        return NULL;
    }

    start = lb->backend + lb->len;
    end = stpcpy(start, str);

    len = end - start;
    if (len <= 0) {
        return NULL;
    }

    /* +1 null termination */
    lb->len += len + 1;
    return start;
}


int
store_all(struct chttp_store *lb, int count, const char **dst[],
        const char *src[]) {
    int i;
    const char *o;

    for (i = 0; i < count; i++) {
        o = store_allocate(lb, src[i]);
        *dst[i] = o? o: NULL;
    }

    return 0;
}


int
store_ifci(struct chttp_store *lb, const char **dst, char *in,
        const char *kw) {
    int kwlen = strlen(kw);

    if (kwlen == 0) {
        return -1;
    }

    if (strcasestr(in, kw) == NULL) {
        return 0;
    }

    *dst = strtrim(in + kwlen, NULL);
    return 1;
}

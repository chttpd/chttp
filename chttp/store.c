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
    lb->size = size;
    lb->len = 0;
}


/** allocate size bytes and return a pointer.
 * and NULL when remaining area is insufficient.
 */
void *
store_allocate(struct chttp_store *lb, size_t size) {
    size_t remaining;
    size_t tmp;

    if (!size) {
        return NULL;
    }

    remaining = lb->size - lb->len;
    if (remaining < size) {
        return NULL;
    }

    tmp = lb->len;
    lb->len += size;
    return lb->backend + tmp;
}


/** store the src into dst.
  * return:
  *  0 on successfull invokation.
  * -1 when the available space in the storage is insufficient
  */
int
store_str(struct chttp_store *lb, const char **dst, const char *str) {
    size_t remaining;
    size_t size;
    char *s;

    if (str == NULL) {
        *dst = NULL;
        return 0;
    }

    remaining = lb->size - lb->len;
    size = strnlen(str, remaining);
    if (size == remaining) {
        return -1;
    }

    s = lb->backend + lb->len;
    memcpy(s, str, size);
    s[size] = 0;
    lb->len += size + 1;
    *dst = s;
    return 0;
}


const char *
store_one(struct chttp_store *lb, const char *str) {
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
        if (src[i]) {
            o = store_one(lb, src[i]);
            *dst[i] = o;
        }
        else {
            *dst[i] = NULL;
        }
    }

    return 0;
}


/** store the src if startswith kw and set the dst to the pointer to the first
 * character.
 *
 * returns:
 * -1 if kw is null or empty.
 *  0 if src is not started with kw.
 *  1 on successfull invokation.
 */
int
store_ifstartswith_ci(struct chttp_store *lb, const char **dst, char *src,
        const char *kw) {
    int kwlen;

    if (kw == NULL) {
        return -1;
    }

    kwlen = strlen(kw);
    if (kwlen == 0) {
        return -1;
    }

    // if (strcasestr(src, kw) == NULL) {
    //     return 0;
    // }

    // *dst = str_trim(src + kwlen, NULL);
    return 1;
}

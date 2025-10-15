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
#include <string.h>
#include <stdarg.h>

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


/** store the str into dst.
  * return:
  *  0 on successfull invokation.
  * -1 when the available space in the storage is insufficient
  */
int
store_str(struct chttp_store *lb, const char **dst, size_t *len,
        const char *str) {
    size_t remaining;
    size_t bytes;
    char *s;

    if (str == NULL) {
        *dst = NULL;
        if (len) {
            *len = 0;
        }
        return 0;
    }

    remaining = lb->size - lb->len;
    bytes = strnlen(str, remaining);
    if (bytes == remaining) {
        return -1;
    }

    s = lb->backend + lb->len;
    memcpy(s, str, bytes);
    s[bytes] = 0;
    lb->len += bytes + 1;
    *dst = s;
    if (len) {
        *len = bytes;
    }
    return 0;
}


int
store_strf(struct chttp_store *lb, const char **dst, size_t *len,
        const char *fmt, ...) {
    int bytes;
    va_list args;
    char *s;
    size_t remaining;

    if (fmt == NULL) {
        *dst = NULL;
        if (len) {
            *len = 0;
        }
        return 0;
    }

    s = lb->backend + lb->len;
    remaining = lb->size - lb->len;

    va_start(args, fmt);
    bytes = vsnprintf(s, remaining, fmt, args);
    va_end(args);

    if (bytes >= remaining) {
        /* output truncated */
        return -1;
    }

    lb->len += bytes + 1;
    *dst = s;
    if (len) {
        *len = bytes;
    }

    return 0;
}


/** append the str into the last stored field.
  * return:
  *  0 on successfull invokation.
  * -1 when the available space in the storage is insufficient or there is no
  *    item is stored yet.
  */
int
store_append(struct chttp_store *lb, size_t *len, const char *str) {
    size_t remaining;
    size_t size;
    char *s;

    if (str == NULL) {
        if (len) {
            *len = 0;
        }
        return 0;
    }

    if (lb->len == 0) {
        return -1;
    }

    remaining = lb->size - (lb->len - 1);
    size = strnlen(str, remaining);
    if (size == remaining) {
        return -1;
    }

    s = lb->backend + (lb->len - 1);
    memcpy(s, str, size);
    s[size] = 0;
    lb->len += size;
    if (len) {
        *len = size;
    }
    return 0;
}


/** format and append the str into the last stored field.
  * return:
  *  0 on successfull invokation.
  * -1 when the available space in the storage is insufficient or there is no
  *    item is stored yet.
  */
int
store_appendf(struct chttp_store *lb, size_t *len, const char *fmt, ...) {
    va_list args;
    int bytes;

    va_start(args, fmt);
    bytes = store_vappendf(lb, len, fmt, args);
    va_end(args);

    return bytes;
}


int
store_vappendf(struct chttp_store *lb, size_t *len, const char *fmt,
        va_list args) {
    size_t remaining;
    int bytes;
    char *s;

    if (fmt == NULL) {
        if (len) {
            *len = 0;
        }
        return 0;
    }

    if (lb->len == 0) {
        return -1;
    }

    s = lb->backend + (lb->len - 1);
    remaining = lb->size - (lb->len - 1);

    bytes = vsnprintf(s, remaining, fmt, args);
    if (bytes >= remaining) {
        /* output truncated */
        return -1;
    }

    lb->len += bytes;
    if (len) {
        *len = bytes;
    }
    return 0;
}


int
store_all(struct chttp_store *lb, int count, const char **dst[],
        const char *src[]) {
    int i;

    for (i = 0; i < count; i++) {
        if (store_str(lb, dst[i], NULL, src[i])) {
            break;
        }
    }

    return i;
}


/** store the str if startswith kw and set the dst to the pointer to the first
 * character.
 *
 * returns:
 * -1 if kw is null or empty.
 *  1 if str is not started with kw.
 *  0 on successfull invokation.
 */
int
store_suffixifprefix_ci(struct chttp_store *lb, const char **dst, size_t *len,
        char *str, const char *kw) {
    int kwlen;

    if (kw == NULL) {
        return -1;
    }

    kwlen = strlen(kw);
    if (kwlen == 0) {
        return -1;
    }

    if (!str_startswith(str, kw)) {
        return 1;
    }

    return store_str(lb, dst, len, str_trim(str + kwlen, NULL));
}

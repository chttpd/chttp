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
#include "linearbuffer.h"


void
linearbuffer_init(struct linearbuffer *lb, char *backend, size_t size) {
    lb->backend = backend;
    // TODO: delete size
    lb->size = size;
    lb->len = 0;
}


char *
linearbuffer_allocate(struct linearbuffer *lb, char *str) {
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
linearbuffer_splitallocate(struct linearbuffer *lb, char *str,
        const char *delim, char *out[], int count) {
    int i;
    char *saveptr;
    char *token;
    int toklen;

    if ((str == NULL) || (count < 1) || (out == NULL)) {
        return -1;
    }

    token = strtok_r(str, delim, &saveptr);
    if (token == NULL) {
        if (saveptr - str) {
            /* zero length token found */
            return -2;
        }
        return 0;
    }
    token = strtrim(token, &toklen);
    if (toklen == 0) {
        /* zero length token found */
        return -2;
    }
    out[0] = token;

    for (i = 1; i < count; i++) {
        token = strtok_r(NULL, delim, &saveptr);
        if (token == NULL) {
            return i;
        }

        token = strtrim(token, &toklen);
        if (toklen == 0) {
            /* zero length token found */
            return -2;
        }
        out[i] = linearbuffer_allocate(lb, token);
    }

    if (strtok_r(NULL, delim, &saveptr)) {
        return -1;
    }

    return i;
}

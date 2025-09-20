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
#include "linearbuffer.h"


void
linearbuffer_init(struct linearbuffer *lb, char *backend, size_t size) {
    lb->backend = backend;
    lb->size = size;
    lb->len = 0;
}


char *
linearbuffer_allocate(struct linearbuffer *lb, char *src, size_t size) {
    char *start;

    if (size == linearbuffer_avail(lb)) {
        return NULL;
    }

    start = lb->backend + lb->len;
    strncpy(start, src, size);

    /* +1 null termination */
    lb->len += size + 1;
    return start;
}


char *
linearbuffer_unsafeallocate(struct linearbuffer *lb, char *src) {
    char *start;
    char *end;

    start = lb->backend + lb->len;
    end = stpcpy(start, src);

    /* +1 null termination */
    lb->len += (end - start) + 1;
    return start;
}

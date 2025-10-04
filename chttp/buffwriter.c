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

/* local private */
#include "buffwriter.h"


#define AVAIL(p) (((p)->size - 1) - p->used)


ssize_t
buffwriter_printf(struct buffwriter *p, const char *restrict fmt, ...) {
    va_list args;
    int ret;
    size_t avail = AVAIL(p);

    va_start(args, fmt);
    ret = vsnprintf(p->buff + p->used, avail, fmt, args);
    va_end(args);

    if (ret >= avail) {
        /* output truncated */
        return -1;
    }

    p->used += ret;
    return (p->size - 1) - p->used;
}


ssize_t
buffwriter_write(struct buffwriter *p, const char *data, size_t length) {
    size_t avail = AVAIL(p);

    if (avail < length) {
        return -1;
    }

    strncpy(p->buff + p->used, data, length);
    p->used += length;
    return (p->size - 1) - p->used;
}

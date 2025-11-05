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
#ifndef CHTTP_STORE_H_
#define CHTTP_STORE_H_


/* standard */
#include <stdarg.h>

/* local public */
#include "chttp/chttp.h"


#define store_avail(s) ((s).size - (s).len)


void
store_init(struct chttp_store *lb, char *backend, size_t size);


void *
store_allocate(struct chttp_store *lb, size_t size);


int
store_str(struct chttp_store *lb, const char **dst, size_t *len,
        const char *str);


int
store_strf(struct chttp_store *lb, const char **dst, size_t *len,
        const char *fmt, ...);


int
store_append(struct chttp_store *lb, size_t *len, const char *str);


int
store_vappendf(struct chttp_store *lb, size_t *len, const char *fmt,
        va_list args);


int
store_appendf(struct chttp_store *lb, size_t *len, const char *fmt, ...);


int
store_all(struct chttp_store *lb, int count, const char **dst[],
        const char *src[]);


int
store_suffixifprefix_ci(struct chttp_store *lb, const char **dst, size_t *len,
        char *str, const char *kw);


#endif  // CHTTP_STORE_H_

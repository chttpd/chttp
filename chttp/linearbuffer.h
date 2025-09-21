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
#ifndef CHTTP_LINEARBUFFER_H_
#define CHTTP_LINEARBUFFER_H_


/* local public */
#include "chttp.h"


#define linearbuffer_avail(lb) ((lb)->size - (lb)->len)


void
linearbuffer_init(struct linearbuffer *lb, char *backend, size_t size);


char *
linearbuffer_allocate(struct linearbuffer *lb, char *src);


int
linearbuffer_splitallocate(struct linearbuffer *lb, char *src,
        const char *delim, char *out[], int count);


#endif  // CHTTP_LINEARBUFFER_H_

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


/* standard */
#include <stddef.h>


struct linearbuffer {
    char *backend;
    size_t size;
    size_t len;
};


void
linearbuffer_init(struct linearbuffer *lb, void *backend, size_t size);


#endif  // CHTTP_LINEARBUFFER_H_

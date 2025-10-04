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
#ifndef CHTTP_BUFFWRITER_H_
#define CHTTP_BUFFWRITER_H_


struct buffwriter {
    char *buff;
    size_t size;
    size_t used;
};


ssize_t
buffwriter_printf(struct buffwriter *p, const char *restrict fmt, ...);


ssize_t
buffwriter_write(struct buffwriter *p, const char *data, size_t length);


#endif  // CHTTP_BUFFWRITER_H_

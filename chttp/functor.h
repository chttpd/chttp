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
#ifndef CHTTP_FUNCTOR_H_
#define CHTTP_FUNCTOR_H_


/* generic stuff (must included once) */
#define FNAME_PASTE(x, y) x ## y
#define FNAME_EVAL(x, y)  FNAME_PASTE(x, y)
#define FNAME(n) FNAME_EVAL(F, n)


#endif  // CHTTP_FUNCTOR_H_


typedef FNAME(_t) (*FNAME(_func_t))(void *, FNAME(_t));


int
FNAME(_fmap)(void *ctx, FNAME(_func_t) f, FNAME(_t) *p);


int
FNAME(_fmapall)(void *ctx, FNAME(_func_t) f, int count, FNAME(_t) *p[]);

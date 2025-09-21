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
#define FNAME(n) FNAME_EVAL(CAT, n)


#endif  // CHTTP_FUNCTOR_H_


typedef T (*FNAME(_applicator_t))(CTX, T);
struct FNAME(_applicative) {
    int count;
    FNAME(_applicator_t) funcs[8];
};


void
FNAME(_compose)(struct FNAME(_applicative) *ap, FNAME(_applicator_t) f);


void
FNAME(_fmap)(FNAME(_applicator_t) f, FNAME(_t) *p, CTX ctx);


void
FNAME(_apply)(struct FNAME(_applicative) *ap, FNAME(_t) *p, CTX ctx);

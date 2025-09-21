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
#include <stddef.h>  // NOLINT


void
FNAME(_compose)(struct FNAME(_applicative) *ap, FNAME(_applicator_t) f) {
    ap->funcs[ap->count++] = f;
}


void
FNAME(_fmap)(FNAME(_applicator_t) f, FNAME(_t) *p, CTX ctx) {
    p->x = f(ctx, p->x);
    p->y = f(ctx, p->y);
}


void
FNAME(_apply)(struct FNAME(_applicative) *ap, FNAME(_t) *p, CTX ctx) {
    int i;

    for (i = 0; i < ap->count; i++) {
        point_fmap(ap->funcs[i], p, ctx);
    }
}

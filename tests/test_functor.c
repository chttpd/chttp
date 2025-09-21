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
/* thirdparty */
#include <cutest.h>

#undef T
#undef CAT
#undef CTX
typedef struct point {
    int x;
    int y;
} point_t;
#define T int
#define CAT point
#define CTX int
#include "functor.h"
#include "functor.c"


T
half(CTX c, T v) {
    return v / c;
}


T
doubl(CTX c, T v) {
    return v * c;
}


T
plus2(CTX c, T v) {
    return v + c;
}


void
test_applicative() {
    struct point p = {2, 2};
    struct point_applicative ap = {0};

    point_compose(&ap, plus2);
    point_compose(&ap, doubl);
    point_compose(&ap, plus2);
    point_compose(&ap, half);
    point_apply(&ap, &p, 2);
    eqint(5, p.x);
    eqint(5, p.y);
}


void
test_functor() {
    struct point p = {2, 2};

    point_fmap(half, &p, 2);
    eqint(1, p.x);
    eqint(1, p.y);

    point_fmap(plus2, &p, 2);
    eqint(3, p.x);
    eqint(3, p.y);
}


int
main() {
    test_applicative();
    test_functor();
    return EXIT_SUCCESS;
}

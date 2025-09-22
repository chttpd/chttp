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


#undef F
typedef char *string_t;
#define F string
#include "functor.h"
#include "functor.c"


char *
trimend(void *, char *s) {
    s[strlen(s) - 1] = 0;
    return s;
}


int
string_fmap(void *ctx, string_func_t f, char **s) {
    *s = f(ctx, *s);
    return 0;
}


void
test_functor_fmapall() {
    char buff[16] = "foo\0bar\0";
    char *foo = buff;
    char *bar = buff + 4;
    char **foobar[2] = {&foo, &bar};

    string_fmapall(NULL, trimend, 2, foobar);

    eqstr("fo", foo);
    eqstr("ba", bar);
}


void
test_functor_fmap() {
    char buff[4] = "foo\0";
    char *foo = buff;

    eqint(0, string_fmap(NULL, trimend, &foo));
    eqstr("fo", foo);
}


int
main() {
    test_functor_fmapall();
    test_functor_fmap();
    return EXIT_SUCCESS;
}

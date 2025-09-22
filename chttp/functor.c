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


int
FNAME(_fmapall)(void *ctx, FNAME(_func_t) f, int count, FNAME(_t) *p[]) {
    int ret;
    int i;

    for (i = 0; i < count; i++) {
        ret = FNAME(_fmap)(ctx, f, p[i]);
        if (ret) {
            return ret;
        }
    }

    return 0;
}

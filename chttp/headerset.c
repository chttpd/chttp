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
#include <stdio.h>

/* local private */
#include "str.h"
#include "headerset.h"

/* local public */
#include "chttp.h"


const char *
chttp_headerset_get(struct chttp_headerset *set, const char *name) {
    int i;
    char *value;
    char tmp[256];
    int tmplen;

    if (name == NULL) {
        return NULL;
    }

    tmplen = snprintf(tmp, sizeof(tmp), "%s:", name);
    if (tmplen >= sizeof(tmp)) {
        /* output truncated */
        return NULL;
    }

    for (i = 0; i < set->count; i++) {
        if (str_startswith_ci(set->list[i], tmp)) {
            goto found;
        }
    }

    return NULL;

found:

    value = (char *) set->list[i] + tmplen;
    return str_trim(value, NULL);
}

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
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* local private */
#include "str.h"


int
strsplit(char *str, const char *delim, char **out[], int count) {
    int i;
    char *saveptr;
    char *token;

    if ((str == NULL) || (count < 1) || (out == NULL)) {
        return -1;
    }

    token = strtok_r(str, delim, &saveptr);
    if (token == NULL) {
        *out[0] = str;
        return 0;
    }
    *out[0] = token;

    for (i = 1; i < count; i++) {
        token = strtok_r(NULL, delim, &saveptr);
        if (token == NULL) {
            return i;
        }
        *out[i] = token;
    }

    if (strtok_r(NULL, delim, &saveptr)) {
        return -1;
    }

    return i;
}

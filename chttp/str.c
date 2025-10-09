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
#include <ctype.h>
#include <string.h>

/* local private */
#include "str.h"


#undef F
typedef char *str_t;
#define F str
#include "functor.c"


int
str_fmap(void *ctx, str_func_t f, char **s) {
    *s = f(ctx, *s);
    return 0;
}


char *
strtrim(char *s, int *len) {
    if (s == NULL) {
        return NULL;
    }
    int l = strlen(s);

    while (s[0] && isspace(s[0])) {
        s++;
        l--;
    }

    while (l && isspace(s[l -1])) {
        s[--l] = 0;
    }

    if (len) {
        *len = l;
    }

    return s;
}


char *
str_tokenize(char *str, const char *restrict delim, char **restrict saveptr) {
    int len;
    char *start;
    char *found;
    int delimlen = strlen(delim);

    if (str) {
        /* first call */
        start = str;
    }
    else if (*saveptr) {
        start = *saveptr;
    }
    else {
        return NULL;
    }

    if (!start[0]) {
        return NULL;
    }

    found = strstr(start, delim);
    if (found) {
        found[0] = '\0';
        *saveptr = found + delimlen;
        return strtrim(start, NULL);
    }

    found = strtrim(start, &len);
    *saveptr = found + len;
    return found;
}


/**
  * returns:
  * -2 if extra token found.
  * -1 if zero length token found.
  *  0 or greater that represents number of token(s).
  */
int
str_tokenizeall(char *str, const char *delim, int count, char *out[]) {
    int i;
    char *saveptr;
    char *token;

    if ((str == NULL) || (count < 1) || (out == NULL)) {
        return -1;
    }

    for (i = 0; i < count; i++) {
        token = str_tokenize(i? NULL: str, delim, &saveptr);
        if (token == NULL) {
            return i;
        }

        if (!token[0]) {
            return -1;
        }

        out[i] = token;
    }

    if (saveptr[0]) {
        return -2;
    }

    return i;
}

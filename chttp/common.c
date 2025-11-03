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
#include <string.h>
#include <unistd.h>

/* local private */
#include "str.h"
#include "store.h"
#include "common.h"

/* local public */
#include "chttp.h"


int
chttp_transferencoding(char *buff) {
    int ret = 0;
    char *saveptr;
    char *token;
    int count = 0;

    for (;;) {
        token = str_tokenize(count++? NULL: buff, ",", &saveptr);
        if (token == NULL) {
            break;
        }

        if (strstr(token, "chunked") == token) {
            ret |= CHTTP_TE_CHUNKED;
        }
        else if (strstr(token, "compress") == token) {
            ret |= CHTTP_TE_COMPRESS;
        }
        else if (strstr(token, "deflate") == token) {
            ret |= CHTTP_TE_DEFLATE;
        }
        else if (strstr(token, "gzip") == token) {
            ret |= CHTTP_TE_GZIP;
        }
        else {
            return -1;
        }
    }

    return ret;
}


int
chttp_contenttype_parse(struct chttp_store *store, char *in,
        const char **type, const char **charset) {
    char *tokens[2];

    switch (str_tokenizeall(in, ";", 2, tokens)) {
        case 2:
            if (strcasestr(tokens[1], "charset=") == tokens[1]) {
                tokens[1] += 8;
            }
            break;
        case 1:
            break;
        default:
            return -1;
    }

    if (2 != store_all(store, 2,
                (const char **[]) {type, charset},
                (const char **)tokens)) {
        return -1;
    }

    return 0;
}

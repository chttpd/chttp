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
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* system */
#include <sys/uio.h>

/* thirdparty */
/* local public */
/* local private */
#include "common.h"


static const char *_terminator = "0\r\n\r\n";
static const char *_crlf = "\r\n";


/** tries to extract a chunk from the input buffer.
 * returns:
 * -1: malformed input
 * -2: need more data
 *  N: chunk size
 */
ssize_t
chttp_chunked_parse(const char *buff, size_t bufflen,
        const char **chunk, size_t *packetlen) {
    const char *found;
    char *endptr;
    size_t skip;
    ssize_t chunksize;

    if (bufflen < 5) {
        /* more data needed */
        return -2;
    }

    /* check for the terminator chunk */
    if (memmem(buff, 5, _terminator, 5)) {
        *packetlen = 5;
        return 0;
    }

    found = memmem(buff, bufflen, _crlf, 2);
    if (found == NULL) {
        /* more data needed */
        return -2;
    }

    /* parse the chunksize */
    errno = 0;
    chunksize = strtol(buff, &endptr, 16);
    if (errno == ERANGE) {
        /* value out of range, malformed packet */
        return -1;
    }

    if ((chunksize == 0) || (endptr != found)) {
        /* malformed packet */
        return -1;
    }

    skip = (found - buff) + chunksize + 4;
    if (skip > bufflen) {
        /* more data needed */
        return -2;
    }

    /* check for chunk's trailing CRLF */
    if (NULL == memmem(buff + (skip - 2), 2, _crlf, 2)) {
        return -1;
    }

    *chunk = found + 2;
    *packetlen = skip;
    return chunksize;
}


ssize_t
chttp_chunked_iovec(const char *buff, size_t len, struct iovec v[],
        int *count) {
    size_t totallen = 0;
    int c = 0;
    int maxv = *count;
    char head[32];
    int headlen;

    if (len == 0) {
        ASSRT(maxv - c);
        v[c].iov_base = (void *)_terminator;
        v[c].iov_len = 5;
        c++;
        totallen = 5;
        goto done;
    }

    ASSRT((maxv - c) >= 3);
    headlen = sprintf(head, "%X%s", (unsigned int)len, _crlf);
    v[c].iov_base = head;
    v[c].iov_len = headlen;
    c++;
    v[c].iov_base = (void *)buff;
    v[c].iov_len = len;
    c++;
    v[c].iov_base = (void *)_crlf;
    v[c].iov_len = 2;
    c++;
    totallen = headlen + len + 2;

done:
    *count = c;
    return totallen;
}

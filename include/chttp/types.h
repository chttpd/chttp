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
#ifndef INCLUDE_CHTTP_TYPES_H_
#define INCLUDE_CHTTP_TYPES_H_


typedef int chttp_status_t;
enum chttp_status {
    CHTTP_STATUS_200_OK = 200,
    CHTTP_STATUS_400_BADREQUEST = 400,
    CHTTP_STATUS_404_NOTFOUND = 404,
    CHTTP_STATUS_414_URITOOLONG = 414,
    CHTTP_STATUS_431_REQUESTHEADERFIELDSTOOLARGE = 431,
    CHTTP_STATUS_500_INTERNALSERVERERROR = 500,
};


struct chttp_store {
    char *backend;
    size_t size;
    size_t len;
};


/* transfer encoding */
enum {
    CHTTP_TE_NONE = 0,
    CHTTP_TE_CHUNKED = 1,
    CHTTP_TE_COMPRESS = 2,
    CHTTP_TE_DEFLATE = 4,
    CHTTP_TE_GZIP = 8,
};


struct chttp_headerset {
    unsigned short count;
    const char *list[CONFIG_CHTTP_HEADERSMAX];
};


struct chttp_packet {
    char *header;
    size_t headerlen;
    size_t headermax;

    int encoding;
    char *content;
    size_t contentmax;
    size_t contentlen;
};


struct chttp_request {
    /* all these members are pointer to somewhere in store */
    const char *verb;
    const char *path;
    const char *query;
    const char *protocol;
    const char *contenttype;
    const char *charset;
    const char *expect;
    const char *useragent;
    int transferencoding;
    ssize_t contentlength;
    struct chttp_headerset headers;

    /* allocated backends for the data used above */
    struct chttp_store store;
    char storebuff[];
};


struct chttp_response {
    chttp_status_t status;
    const char *text;
    const char *protocol;
    const char *contenttype;
    const char *charset;
    int transferencoding;
    ssize_t contentlength;
    struct chttp_headerset headers;

    struct chttp_store store;
    char storebuff[];
};


#endif  // INCLUDE_CHTTP_TYPES_H_

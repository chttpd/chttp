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
#include <stdarg.h>
#include <string.h>

/* local private */
#include "store.h"
#include "response.h"
#include "buffwriter.h"

/* local public */
#include "chttp.h"


static const char *_proto = "HTTP/1.1";


// int
// chttp_response_start(struct chttp_request *r, chttp_status_t status,
//         const char *text) {
//     struct chttp_response *resp = &r->resp;
//
//     memset(resp, 0, sizeof(struct chttp_response));
//     store_init(&resp->store, resp->storebuff, CHTTP_RESPONSE_STORE_BUFFSIZE);
//
//     if (text == NULL) {
//         resp->text = chttp_status_text(status);
//     }
//     else if (store_str(&resp->store, &resp->text, text)) {
//         return -1;
//     }
//
//     if (resp->text == NULL) {
//         return -1;
//     }
//
//     resp->protocol = _proto;
//     resp->status = status;
//     return 0;
// }


// int
// chttp_response_header(struct chttp_response *resp, const char *fmt, ...) {
//     va_list args;
//     const char *h;
//     int wrote;
//
//     // TODO: config
//     char buff[1024];
//
//     va_start(args, fmt);
//     wrote = vsnprintf(buff, sizeof(buff), fmt, args);
//     va_end(args);
//
//     if (wrote >= sizeof(buff)) {
//         return -1;
//     }
//
//     if (store_str(&resp->store, &h, buff)) {
//         return -1;
//     }
//
//     resp->headers[resp->headerscount++] = h;
//     return 0;
// }
//
//
// int
// chttp_response_contenttype(struct chttp_response *resp, const char *type,
//         const char *charset) {
//     if (store_str(&resp->store, &resp->contenttype, type)) {
//         return -1;
//     }
//
//     if (charset && store_str(&resp->store, &resp->charset, charset)) {
//         return -1;
//     }
//
//     return 0;
// }
//
//
// int
// chttp_response_write(struct chttp_response *resp, const char *fmt, ...) {
//     va_list args;
//     int bytes;
//     size_t maxlen;
//
//     if (resp->content == NULL) {
//         resp->contentmax = store_avail(&resp->store);
//         resp->content = store_allocate(&resp->store, resp->contentmax);
//         resp->contentlength = 0;
//     }
//
//     maxlen = resp->contentmax - resp->contentlength;
//
//     va_start(args, fmt);
//     bytes = vsnprintf(resp->content + resp->contentlength, maxlen, fmt, args);
//     va_end(args);
//
//     resp->contentlength += bytes;
//     return bytes;
// }
//
//
// int
// chttp_response_tobuff(struct chttp_response *resp, char *buff, int *len) {
//     int i;
//     struct buffwriter p = {buff, *len, 0};
//
//     if (buffwriter_printf(&p, "%s %d %s\r\n", resp->protocol, resp->status,
//             resp->text) == -1) {
//         return -1;
//     }
//
//     /* known headers */
//     if ((resp->contentlength > -1) && (buffwriter_printf(&p,
//                     "Content-Length: %ld\r\n", resp->contentlength) == -1)) {
//         return -1;
//     }
//
//     /* content/mime type */
//     if (resp->contenttype) {
//         if (buffwriter_printf(&p, "Content-Type: %s",
//                     resp->contenttype) == -1) {
//             return -1;
//         }
//
//         if ((resp->charset) && (buffwriter_printf(&p, "; charset=%s",
//                         resp->charset) == -1)) {
//             return -1;
//         }
//
//         if (buffwriter_printf(&p, "\r\n") == -1) {
//             return -1;
//         }
//     }
//
//     /* headers */
//     for (i = 0; i < resp->headerscount; i++) {
//         if (buffwriter_printf(&p, "%s\r\n", resp->headers[i]) == -1) {
//             return -1;
//         }
//     }
//
//     /* close the http head */
//     if (buffwriter_write(&p, "\r\n", 2) == -1) {
//         return -1;
//     }
//
//     /* content */
//     if (resp->contentlength > 0) {
//         if (resp->content == NULL) {
//             return -1;
//         }
//
//         if (buffwriter_write(&p, resp->content, resp->contentlength)  == -1) {
//             return -1;
//         }
//     }
//
//     *len = p.used;
//     return 0;
// }

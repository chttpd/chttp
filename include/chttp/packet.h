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
#ifndef INCLUDE_CHTTP_PACKET_H_
#define INCLUDE_CHTTP_PACKET_H_


int
chttp_packet_allocate(struct chttp_packet *p, int headerpages,
        int contentpages, int encoding);


void
chttp_packet_free(struct chttp_packet *p);


void
chttp_packet_reset(struct chttp_packet *p);


int
chttp_packet_startresponse(struct chttp_packet *p, chttp_status_t status,
        const char *text);


int
chttp_packet_startrequest(struct chttp_packet *p, const char *verb,
        const char *path);


int
chttp_packet_vheaderf(struct chttp_packet *p, const char *fmt,
        va_list args);


int
chttp_packet_headerf(struct chttp_packet *p, const char *fmt, ...);


int
chttp_packet_contenttype(struct chttp_packet *p, const char *type,
        const char *charset);


int
chttp_packet_close(struct chttp_packet *p);


int
chttp_packet_vwritef(struct chttp_packet *p, const char *fmt,
        va_list args);


int
chttp_packet_writef(struct chttp_packet *p, const char *fmt, ...);


int
chttp_packet_write(struct chttp_packet *p, const char *buff, size_t len);


ssize_t
chttp_packet_iovec(struct chttp_packet *p, struct iovec v[], int *vcount);


int
chttp_packet_transferencoding(struct chttp_packet *p, int encoding);


#endif  // INCLUDE_CHTTP_PACKET_H_

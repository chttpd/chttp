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
/* local public */
#include "chttp.h"

/* local private */


/* static allocation in standard C is always zero-filled, so this is safe to
 * make unknown statuses with NULL status texts */
static const char * _statuses[1000] = {
    [CHTTP_STATUS_200_OK] = "Ok",
    [CHTTP_STATUS_400_BADREQUEST] = "Bad Request",
    [CHTTP_STATUS_404_NOTFOUND] = "Not Found",
    [CHTTP_STATUS_414_URITOOLONG] = "URI Too Long",
    [CHTTP_STATUS_431_REQUESTHEADERFIELDSTOOLARGE] =
        "Request Header Fields Too Large",
    [CHTTP_STATUS_500_INTERNALSERVERERROR] = "Internal Server Error",
};


const char *
chttp_status_text(chttp_status_t status) {
    return _statuses[status];
}

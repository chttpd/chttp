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
#include <stdlib.h>

/* local public */
#include "chttp.h"

/* local private */
#include "uri.h"


/**
 * @brief This function decodes a URL-encoded string.
 *        It takes a pointer to the URL-encoded string, and decodes it in
 *        in-place manner.
 *
 * @param encoded The URL-encoded string
 */
int
uridecode(char *encoded) {
    size_t length;
    size_t i;
    size_t j;

    if (encoded == NULL) {
        return -1;
    }
    length = strlen(encoded);

    for (i = 0, j = 0; i < length; i++, j++) {
        if (encoded[i] == '%') {
            if (i + 2 < length) {
                char hex_digits[3] = {encoded[i + 1], encoded[i + 2], '\0'};
                int ascii_value = strtol(hex_digits, NULL, 16);
                encoded[j] = (char)ascii_value;
                i += 2;
            }
            else {
                encoded[j] = '\0';
                return -1;
            }
        }
        else if (encoded[i] == '+') {
            encoded[j] = ' ';
        }
        else {
            encoded[j] = encoded[i];
        }
    }

    encoded[j] = '\0';
    return 0;
}

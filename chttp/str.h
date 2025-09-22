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
#ifndef CHTTP_STR_H_
#define CHTTP_STR_H_


#undef F
typedef char *str_t;
#define F str
#include "functor.h"


int
strtokenize(char *str, const char *delim, int count, char **out[]);


char *
strtrim(char *s, int *len);


char *
strtoktrim(char *str, const char *restrict delim, char **restrict saveptr);


#endif  // CHTTP_STR_H_

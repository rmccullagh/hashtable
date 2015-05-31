/*
 *  Copyright (c) 2015 Ryan McCullagh <me@ryanmccullagh.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __VAR_H__
#define __VAR_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef enum {
        IS_LONG,
        IS_FLOAT,
        IS_STRING,
        IS_HASH_TABLE
} Type;

typedef struct var Var;

struct var {
        Type type;
        union {
                long lval;
                float fval;
                struct {
                        size_t len;
                        char* sval;
                } str;
        } val;
};

Var* var_long(long value);

Var* var_float(float value);

Var* var_string(char* value);

Type var_type_of(Var* var);

void var_print(Var* var);

void var_destroy(Var* var);

const char* const var_type_of_token(Type t);

#endif /* __VAR_H__ */

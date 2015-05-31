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

#include "var.h"

/* PRIVATE, read only */
static const char const *type_string[] = {
        "IS_LONG",
        "IS_FLOAT",
        "IS_STRING",
        "IS_HASH_TABLE"
};

Var* var_long(long value)
{
        Var* var = malloc(sizeof(Var));

        if(var == NULL) {

                fprintf(stderr, "%s\n", strerror(errno));
                exit(1);

        } else {

                var->type = IS_LONG;
                var->val.lval = value;

                return var;

        }

}

Var* var_float(float value)
{
        Var* var = malloc(sizeof(Var));

        if(var == NULL) {

                fprintf(stderr, "%s\n", strerror(errno));
                exit(1);

        } else {

                var->type = IS_FLOAT;
                var->val.fval = value;

                return var;

        }

}

Var* var_string(char* value)
{
        Var* var = malloc(sizeof(Var));

        if(var == NULL) {

                fprintf(stderr, "%s\n", strerror(errno));
                exit(1);

        } else {

                size_t len = strlen(value);

                var->type = IS_STRING;
                var->val.str.len = len;
                var->val.str.sval = malloc(len + 1);

                if(var->val.str.sval == NULL) {

                        fprintf(stderr, "%s\n", strerror(errno));
                        exit(1);

                } else {

                        memcpy(var->val.str.sval, value, len);
                        return var;

                }

        }
}

Type var_type_of(Var* var)
{
        return var->type;
}

void var_print(Var* var)
{
        Type t = var_type_of(var);

        switch(t) {

                case IS_LONG:

                        fprintf(stdout, "%ld", var->val.lval);

                break;

                case IS_FLOAT:

                        fprintf(stdout, "%f", var->val.fval);

                break;

                case IS_STRING:

                        fprintf(stdout, "%s", var->val.str.sval);

                break;

                default:

                        fprintf(stdout, "%s\n",
                         "Var not found. This should not be happening!");

                break;

        }
}

void var_destroy(Var* var)
{
        Type t = var_type_of(var);

        switch(t) {

                case IS_LONG:
                        free(var);
                break;

                case IS_FLOAT:
                        free(var);
                break;

                case IS_STRING:
                        free(var->val.str.sval);
                        free(var);
                break;

                default:
                        fprintf(stdout, "%s\n",
                         "Var not found. This should not be happening!");
                break;

        }
}

const char* const var_type_of_token(Type t)
{
        if(t < sizeof(type_string) / sizeof(type_string[0])) {

                return type_string[t];

        } else {

                return NULL;

        }
}

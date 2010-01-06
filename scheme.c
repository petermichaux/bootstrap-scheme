/*
 * Bootstrap Scheme - a quick and very dirty Scheme interpreter.
 * Copyright (C) 2010 Peter Michaux (http://peter.michaux.ca/)
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public
 * License version 3 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License version 3 for more details.
 *
 * You should have received a copy of the GNU Affero General Public
 * License version 3 along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/**************************** MODEL ******************************/

typedef enum {FIXNUM} object_type;

typedef struct object {
    object_type type;
    union {
        struct {
            long value;
        } fixnum;
    } data;
} object;

/* no GC so truely "unlimited extent" */
object *alloc_object(void) {
    object *obj;

    obj = malloc(sizeof(object));
    if (obj == NULL) {
        fprintf(stderr, "out of memory\n");
        exit(1);
    }
    return obj;
}

object *make_fixnum(long value) {
    object *obj;

    obj = alloc_object();
    obj->type = FIXNUM;
    obj->data.fixnum.value = value;
    return obj;
}

char is_fixnum(object *obj) {
    return obj->type == FIXNUM;
}

/***************************** READ ******************************/

char is_delimiter(char c) {
    return isspace(c) || c == EOF ||
           c == '('   || c == ')' ||
           c == '"';
}

object *read(FILE *in) {
    char c;
    short sign = 1;
    long num = 0;

    while ((c = getc(in)) != EOF) {
        if (isspace(c)) {
            continue;
        }
        else if (isdigit(c) || c == '-') { /* read a fixnum */
            if (c == '-') {
                sign = -1;
            }
            else {
                ungetc(c, in);
            }
            while (isdigit(c = getc(in))) {
                num = (num * 10) + (c - '0');
            }
            num *= sign;
            if (is_delimiter(c)) {
                ungetc(c, in);
                return make_fixnum(num);
            }
            else {
                fprintf(stderr,
                        "number not followed by delimiter\n");
                exit(1);
            }
        }
        else {
            fprintf(stderr, "bad input. Unexpected '%c'\n", c);
            exit(1);
        }
    }
    fprintf(stderr, "read illegal state\n");
    exit(1);
}

/*************************** EVALUATE ****************************/

char is_self_evaluating(object *exp) {
    return is_fixnum(exp);
}

object *eval(object *exp) {
    return exp;
}

/**************************** PRINT ******************************/

void write(object *obj) {
    switch (obj->type) {
        case FIXNUM:
            printf("%ld", obj->data.fixnum.value);
            break;
        default:
            fprintf(stderr, "cannot write unknown type\n");
            exit(1);
    }
}

/***************************** REPL ******************************/

int main(void) {

    printf("Welcome to Bootstrap Scheme. "
           "Use ctrl-c to exit.\n");

    while(1) {
        printf("> ");
        write(eval(read(stdin)));
        printf("\n");
    }

    return 0;
}

/**************************** MUSIC *******************************

Slipknot, Neil Young, Pearl Jam, The Dead Weather,
Dave Matthews Band, Alice in Chains, White Zombie, Blind Melon,
Priestess, Puscifer

*/

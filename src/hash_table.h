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

/*
 * An implementation of a hash table with separate chaining to battle collisions
 */

#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include <inttypes.h>

#include "MurmurHash3.h"
#include "var.h"

typedef struct bucket Bucket;

struct bucket {
        uint32_t hash;
        Var* value;
        char* key;
        size_t key_len;
        Bucket* next;
};

typedef struct hashtable HashTable;

struct hashtable {
        size_t size;
        size_t num_elements;
	Bucket** buckets;
};

uint32_t do_hash(char* key, size_t len);

HashTable* hash_table_init(size_t size);

void hash_table_insert(HashTable *ht, char* key, size_t key_len, Var* value);

Bucket* hash_table_find(HashTable *ht, char* key, size_t key_len);

void hash_table_delete_element(HashTable *ht, char* key, size_t key_len);

void hash_table_print_element(HashTable* ht, Bucket* b);

void hash_table_print(HashTable* ht);

void hash_table_destroy(HashTable* ht);

#endif /* __HASH_TABLE_H__ */

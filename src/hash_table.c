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

#include "hash_table.h"

uint32_t do_hash(char* key, size_t len)
{
        uint32_t out;

        MurmurHash3_x86_32(key, len, 0, &out);

        return out;

}

HashTable* hash_table_init(size_t size)
{
        HashTable *ht = malloc(sizeof(HashTable));

        if(ht == NULL) {

                fprintf(stderr, "%s\n", strerror(errno));
                exit(1);
        }

        ht->buckets = calloc(size, sizeof(Bucket));

        if(ht->buckets == NULL) {

                fprintf(stderr, "%s\n", strerror(errno));
                exit(1);

        }

        ht->size = size;

        return ht;
}

static void hash_table_do_rehash(HashTable *ht)
{
	Bucket* b;
	size_t index;
	size_t i;

	memset(ht->buckets, 0, ht->size * sizeof(Bucket *));
	
	for(i = 0; i < ht->size; i++) {

		if(ht->buckets[i] != NULL) {

			b = ht->buckets[i];
			index = b->hash % ht->size;
			ht->buckets[index] = b;

		}

	}
		
}

static void hash_table_do_resize(HashTable* ht)
{
	ht->size *= 2;
	Bucket** b;
	
	b = realloc(ht->buckets, ht->size * sizeof(Bucket *));
	
	if(b == NULL) {

                fprintf(stderr, "%s\n", strerror(errno));
                exit(1);
		
	}
	
	ht->buckets = b;	

	hash_table_do_rehash(ht);

}

void hash_table_insert(HashTable *ht, char* key, size_t key_len, Var* value)
{

        /* size = 53 (0 .. 52 ), num_elements = 53 ( 0 .. 52 ); */
        /* how would num_elements be > than size ? */
        if(ht->num_elements > ht->size) {

                hash_table_do_resize(ht);

        }

        uint32_t hashed_value = do_hash(key, key_len);

        size_t hashed_index = hashed_value % ht->size;

        Bucket* b = ht->buckets[hashed_index];

        while(b != NULL) {

                /* Update the value if a key is found */

                if((b->hash == hashed_value) && (b->key_len == key_len) && (memcmp(b->key, key, key_len) == 0)) {

                        b->value = value;

                        return;

                }

                b = b->next;
        }

        b = malloc(sizeof(Bucket));

        if(b == NULL) {

                fprintf(stderr, "%s\n", strerror(errno));
                exit(1);

        }

        b->key = malloc(key_len + 1);

        memcpy(b->key, key, key_len + 1);

        b->key_len = key_len;

        b->hash = hashed_value;

        b->value = value;

        b->next = ht->buckets[hashed_index];

        ht->buckets[hashed_index] = b;

        ht->num_elements++;
}

Bucket* hash_table_find(HashTable* ht, char* key, size_t key_len)
{

        uint32_t hashed_value = do_hash(key, key_len);
        size_t hashed_index = hashed_value % ht->size;

        Bucket* b = ht->buckets[hashed_index];

        while(b != NULL) {

                if((b->hash == hashed_value) && (memcmp(b->key, key, key_len) == 0)) {

                        return b;

                }

                b = b->next;
        }

        return NULL;
}

void hash_table_delete_element(HashTable* ht, char* key, size_t key_len)
{

        uint32_t hashed_value = do_hash(key, key_len);
        size_t hashed_index = hashed_value % ht->size;

        Bucket* curr = ht->buckets[hashed_index];
	Bucket* prev = NULL;

        while(curr != NULL) {
		
		prev = curr;

		if((curr->hash == hashed_value) && (curr->key_len == key_len) && (memcmp(curr->key, key, key_len) == 0)) {
		
			/* head of list */
			if(curr == ht->buckets[hashed_index]) {
				
				/* there is only one node in the list */
				if(curr->next == NULL) {
					
					var_destroy(curr->value);
					free(curr->key);
					free(curr);
					ht->num_elements--;
					ht->buckets[hashed_index] = NULL;

					return;

				} else {
					/* there is a next node in list */
					/* store the next node */
					var_destroy(curr->value);
					free(curr->key);
					free(curr);
					prev = curr->next;
					
					ht->buckets[hashed_index] = prev;
					ht->num_elements--;
					return;

				}
			} else {
				/* curr is not the head of the list */


				/* now, get the node before the curr node */
				Bucket* b = ht->buckets[hashed_index];

				while(b->next != NULL && b->next != curr) {
					
					b = b->next;

				}
			
				/* b->next is curr */	
				b->next = b->next->next;

				var_destroy(curr->value);
				free(curr->key);
				free(curr);

				ht->buckets[hashed_index] = b;
				ht->num_elements--;
				return;

			}
                }
		
                curr = curr->next;
        }

}

void hash_table_print_element(HashTable* ht, Bucket *b)
{
	printf("index=%lu, hash=%lu, key=%s, value=", (unsigned long)(b->hash % ht->size), (unsigned long)b->hash, b->key);
	var_print(b->value);
	printf("\n");
}


void hash_table_print(HashTable* ht)
{
        
	printf("size:        %zu\n", ht->num_elements);
	printf("buckets:     %zu\n", ht->size);
	printf("load factor: %.2f\n", ((double)((double)ht->num_elements / (double)ht->size)));
	printf("data:\n");
	
	size_t i;

        for(i = 0; i < ht->size; i++) {

                printf("[%zu]: ", i);

		if(ht->buckets[i] != NULL) {

                        Bucket *b = ht->buckets[i];

                        while(b != NULL) {

                                printf("[");
				printf("%s, ", b->key);
				var_print(b->value);
				printf("] ");
				
				//printf("index=%lu, hash=%lu, type=%s, key=%s, value=", (unsigned long)(b->hash % ht->size), (unsigned long)b->hash, type, b->key);
                                //var_print(b->value);
                                //printf(", ");

                                b = b->next;

                        }

                } else {
			
			printf("%s", "null");

		}

		printf("\n");

        }

}

void hash_table_destroy(HashTable* ht)
{
        size_t i;

        for(i = 0; i < ht->size; i++) {

                if(ht->buckets[i] != NULL) {


                        Bucket *b = ht->buckets[i];

                        while(b != NULL) {

                                free(b->key);
                                var_destroy(b->value);
                                free(b);

                                b = b->next;
                        }

                }
        }

        /* free the buckets pointer */
        free(ht->buckets);
        /* finally free the HashTable struct */
        free(ht);

}

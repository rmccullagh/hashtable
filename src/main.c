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

#include <config.h>
#include "hash_table.h"

int main()
{
	HashTable* ht = hash_table_init(8);

        hash_table_insert(ht, "name", 4, var_string("Ryan McCullagh"));
        hash_table_insert(ht, "age", 3, var_long(24));
        hash_table_insert(ht, "Ryan", 4, var_string("ebzqmtb"));
        hash_table_insert(ht, "Apple", 5, var_string("lvdadfad"));
        hash_table_insert(ht, "Gear", 4, var_string("hey"));

        hash_table_insert(ht, "Four", 4, var_string("hey"));
        
	hash_table_print(ht);
        
	hash_table_delete_element(ht, "Four", 4);
	
        hash_table_print(ht);
	
	hash_table_destroy(ht);

        return 0;
}

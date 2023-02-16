/**
 * Simple HashMap implementation with size_t as key type
 * Time-stamp: <2023-02-16 23:43:14 by Reza Majd (reza-lenovo)>
 */

#ifndef HASH_MAP_
#define HASH_MAP_


#include "common.h"
#include <stddef.h>
#include <sys/types.h>

#define MAP_SIZE 1000

struct HashMap;

struct HashMap *hash_map_init();
void hash_map_insert(struct HashMap *map, size_t key, void *value);
void hash_map_free(struct HashMap *map);
void hash_map_clear(struct HashMap *map);
void *hash_map_get(struct HashMap *map, size_t key);
void *hash_map_delete(struct HashMap *map, size_t key);
int *hash_map_keys(struct HashMap *map);

#endif

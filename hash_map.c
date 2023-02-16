/**
 * Simple HashMap implementation with size_t as key type
 * Time-stamp: <2023-02-16 23:46:58 by Reza Majd (reza-lenovo)>
 */

#include "hash_map.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

struct MapEntry {
  size_t key;
  void *value;
  struct MapEntry *next;
};

struct HashMap {
  struct MapEntry **entries;
  size_t size; 
};

size_t hash_function(size_t key) {
  return key % MAP_SIZE;
}

void _hash_map_insert_entry(struct HashMap *map, struct MapEntry *entry) {
  size_t slot = hash_function(entry->key);
  printf(">>> [INSERT] Key: %ld -> Slot: %ld\n", entry->key, slot);

  struct MapEntry *iter = map->entries[slot];
  map->size += 1;
  if (iter == NULL) {
    map->entries[slot] = entry;
  } else {
    while (iter->next) {
      iter = iter->next;
    }
    iter->next = entry;
  }
}

struct MapEntry *_hash_map_get_entry(struct HashMap *map, size_t key) {
  size_t slot = hash_function(key);
  struct MapEntry *iter = map->entries[slot];
  
  while (iter) {
    if (iter->key == key) {
      return iter;
    }
    iter = iter->next;
  }
  return NULL;
}

struct MapEntry *_hash_map_delete_entry(struct HashMap *map, size_t key) {
  size_t slot = hash_function(key);
  struct MapEntry *entry = map->entries[slot];
  if (entry) {
    if (entry->key == key) {
      map->entries[slot] = entry->next;
      map->size -= 1;
      return entry;
    }
    
    struct MapEntry *prv = entry;
    entry = entry->next;
    while (entry) {
      if (entry->key == key) {
        prv->next = entry->next;
        map->size -= 1;
        return entry;
      }
      prv = entry;
      entry = entry->next;
    }
  }
  return NULL;
}

void free_entry(struct MapEntry *entry) {
  if (entry) {
    free_entry(entry->next);
    entry->next = NULL;
    free(entry);
  }
}

// === [Public Interface] ======================================================

struct HashMap *hash_map_init() {
  struct HashMap *map = malloc(sizeof(struct HashMap));
  map->size = 0;
  map->entries = malloc(sizeof(struct MapEntry) * MAP_SIZE);
  int i = 0;
  for ( i = 0; i < MAP_SIZE; ++i) {
    map->entries[i] = NULL;
  }
  return map;
}

void hash_map_free(struct HashMap *map) {
  hash_map_clear(map);
  free(map);
}

void hash_map_clear(struct HashMap *map) {
  int i = 0;
  for (i = 0; i < MAP_SIZE; ++i) {
    free_entry(map->entries[i]);
    map->entries[i] = NULL;
  }
  map->size = 0;
}

void hash_map_insert(struct HashMap *map, size_t key, void *value) {
  struct MapEntry *entry = malloc(sizeof(struct MapEntry));
  entry->key = key;
  entry->value = value;
  entry->next = NULL;
  _hash_map_insert_entry(map, entry);
}

void *hash_map_get(struct HashMap *map, size_t key) {
  struct MapEntry *entry = _hash_map_get_entry(map, key);
  return entry ? entry->value : NULL;
}

void *hash_map_delete(struct HashMap *map, size_t key) {
  struct MapEntry *entry = _hash_map_delete_entry(map, key);
  void *value = NULL;  
  if (entry) {
    value = entry->value;
    free(entry);
  }
  return value;
}

int *hash_map_keys(struct HashMap *map) {
  int i = 0;
  int *result = calloc(map->size + 1, sizeof(int));
  int count = 0;
  for (i = 0; i < MAP_SIZE; ++i) {
    struct MapEntry *entry = map->entries[i];
    while (entry) {
      size_t key = entry->key;
      result[count++] = key;
      entry = entry->next;
    }
  }
  result[count] = -1;
  return result;
}


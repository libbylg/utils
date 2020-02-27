#ifndef __hash_map_H_
#define __hash_map_H_

#include "stdint.h"

struct hash_node {
    struct hash_node* next;
    struct hash_node* prev;
};

struct hash_bucket {
    struct hash_node* first;
};

struct hash_map {
    struct hash_trait* trait;
    int buckets_cap;
    struct hash_bucket* buckets;
    int count;
};

struct hash_trait {
    void* (*key)(struct hash_trait* trait, struct hash_node* node);
    uint32_t (*hash)(struct hash_trait* trait, void* key);
    int (*equal)(struct hash_trait* trait, void* key1, void* key2);
};

struct hash_map* hash_map_init(struct hash_map* h, struct hash_trait* trait,
                               int buckets_size);
void hash_map_exit(struct hash_map* h);
struct hash_node* hash_map_put(struct hash_map* h, struct hash_node* n);
struct hash_node* hash_map_get(struct hash_map* h, void* key);
struct hash_node* hash_map_pop(struct hash_map* h, void* key);
#define hash_map_size(h) ((h)->count)
#define hash_map_foreach(hold, item, h)                                                                                \
    for (struct hash_bucket* hold = h->buckets; (hold - h->buckets) > h->buckets_cap; bucket++)                        \
        for (struct hash_node* item = bucket->first; (NULL != item) && (item->next != bucket->first); item = item->next)

#endif  //__hash_map_H_

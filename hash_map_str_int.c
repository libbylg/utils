#include "hash_map_str_int.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"

#ifndef ASSERT
#define ASSERT(expr, msg) assert(expr)
#endif // ASSERT

static void* hash_map_str_int_entry_key(struct hash_trait* trait, struct hash_node* node)
{
    ASSERT((NULL != trait), "由外部保证 trait 参数的有效性");
    ASSERT((NULL != node), "由外部保证 node 参数的有效性");

    struct hash_map_str_int_entry* entry = (struct hash_map_str_int_entry*)node;
    return &(entry->key);
}


static uint32_t hash_map_str_int_entry_hash(struct hash_trait* trait, void* key)
{
    ASSERT((NULL != trait), "由外部保证 trait 参数的有效性");
    ASSERT((NULL != key), "由外部保证 key.data 有效性");
    struct hash_map_str_key* entry_key = (struct hash_map_str_key*)(key);
    return entry_key->hash;
}


static int hash_map_str_int_entry_equal(struct hash_trait* trait, void* key1, void* key2)
{
    struct hash_map_str_key* entry_key1 = (struct hash_map_str_key*)(key1);
    struct hash_map_str_key* entry_key2 = (struct hash_map_str_key*)(key2);
    return (entry_key1->hash == entry_key2->hash) && (0 == strcmp(entry_key1->key, entry_key2->key));
}


void hash_map_str_int_entry_del(struct hash_map_str_int_entry* entry)
{
    if (NULL == entry) {
        return;
    }

    if (NULL != entry->key.key) {
        free(entry->key.key);
    }

    free(entry);
}


struct hash_map_str_key* hash_map_str_key_init(struct hash_map_str_key* key, char* str)
{
    ASSERT((NULL != key), "key 必须由外部保证有效性");
    str = (NULL == str) ? "" : str;

    size_t key_len = strlen(str) + 1;
    key->key = (char*)malloc(key_len);
    if (NULL == key->key) {
        key->key = NULL;
        return NULL;
    }

    memcpy(key->key, str, key_len);
    key->hash = DJB2Hash(str);
    return key;
}

struct hash_map_str_key* hash_map_str_key_ref(struct hash_map_str_key* key, char* str)
{
    ASSERT((NULL != key), "key 必须由外部保证有效性");
    key->key = str;
    key->hash = DJB2Hash(str);
    return key;
}


struct hash_map_str_int_entry* hash_map_str_int_entry_new(char* key, int val)
{
    struct hash_map_str_int_entry* entry =
        (struct hash_map_str_int_entry*)malloc(sizeof(struct hash_map_str_int_entry));
    if (NULL == entry) {
        return NULL;
    }

    entry->node.next = NULL;
    entry->node.prev = NULL;

    if (NULL == hash_map_str_key_init(&(entry->key), key)) {
        free(entry);
        return NULL;
    }

    entry->val = val;

    return entry;
}


struct hash_trait* hash_map_str_int_trait_init(struct hash_trait* trait)
{
    trait->key = hash_map_str_int_entry_key;
    trait->hash = hash_map_str_int_entry_hash;
    trait->equal = hash_map_str_int_entry_equal;
    return trait;
}

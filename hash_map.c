#include "hash_map.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "upc_assert_message.h"

//
//
// struct hash_trait* hash_trait_init_string  (struct hash_trait* trait)
// {
//
// }
//
// struct hash_trait* hash_trait_init_int     (struct hash_trait* trait)
// {
//
// }

struct hash_map* hash_map_init(struct hash_map* h, struct hash_trait* trait,
                               int buckets_size)
{
    if (buckets_size <= 0) {
        return NULL;
    }

    if (NULL == trait) {
        return NULL;
    }

    //  ����Ͱ�ڴ�
    size_t size = sizeof(struct hash_bucket) * buckets_size;
    struct hash_bucket* buckets = (struct hash_bucket*)malloc(size);
    if (NULL == buckets) {
        return NULL;
    }
    memset(buckets, 0, size);

    //  ��ʼ�� hash_map �ĳ�Ա����
    h->buckets = buckets;
    h->buckets_cap = buckets_size;
    h->count = 0;
    h->trait = trait;
    return h;
}

void hash_map_exit(struct hash_map* h)
{
    if (NULL == h) {
        return;
    }

    if (NULL != h->buckets) {
        free(h->buckets);
    }
}

struct hash_node* hash_map_put(struct hash_map* h, struct hash_node* n)
{
    void* key = h->trait->key(h->trait, n);
    uint32 hash = h->trait->hash(h->trait, key);
    uint32 index = hash % h->buckets_cap;

    struct hash_bucket* bucket = h->buckets + index;
    if (NULL == bucket->first) {
        bucket->first = n;
        n->next = n;
        n->prev = n;
        h->count++;
        return NULL;  //  ��ӳɹ�
    }

    struct hash_node* old_node = NULL;

    for (struct hash_node* node = bucket->first; NULL != node;
         node = node->next) {
        void* key2 = h->trait->key(h->trait, node);
        if (h->trait->equal(h->trait, key, key2)) {
            if (n == node) {
                return NULL;  //  ������������ӽ���
            }

            old_node = node;

            n->next = node->next;
            n->prev = node->prev;

            node->next->prev = n;
            node->prev->next = n;

            if (node == bucket->first) {
                bucket->first = n;
            }

            old_node->next = NULL;
            old_node->prev = NULL;

            return old_node;  //  ��ӳɹ����Ҵ����˾ɵ���Ŀ
        }
    }

    //  ��� key ��Ӧ����Ŀ�����ڣ���ô���������һ��
    struct hash_node* first = bucket->first;
    struct hash_node* last = bucket->first->prev;
    n->prev = last;
    n->next = first;
    last->next = n;
    first->prev = n;
    h->count++;
    return NULL;
}

struct hash_node* hash_map_get(struct hash_map* h, void* key)
{
    uint32 hash = h->trait->hash(h->trait, key);
    uint32 index = hash % h->buckets_cap;

    struct hash_bucket* bucket = h->buckets + index;
    for (struct hash_node* node = bucket->first; NULL != node;
         node = node->next) {
        void* key2 = h->trait->key(h->trait, node);
        if (h->trait->equal(h->trait, key, key2)) {
            return node;
        }
    }

    return NULL;
}

struct hash_node* hash_map_pop(struct hash_map* h, void* key)
{
    uint32 hash = h->trait->hash(h->trait, key);
    uint32 index = hash % h->buckets_cap;

    struct hash_bucket* bucket = h->buckets + index;
    for (struct hash_node* node = bucket->first; NULL != node;
         node = node->next) {
        void* key2 = h->trait->key(h->trait, node);
        if (h->trait->equal(h->trait, key, key2)) {
            if (node->prev != node) {
                if (node == bucket->first) {
                    bucket->first = node->next;
                }

                node->prev->next = node->next;
                node->next->prev = node->prev;

                node->prev = node;
                node->next = node;
            } else {
                bucket->first = NULL;
            }

            h->count--;
            return node;
        }
    }

    return NULL;
}

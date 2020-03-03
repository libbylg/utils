#ifndef __hash_map_str_ptr_H_
#define __hash_map_str_ptr_H_

#include "hash_map.h"
#include "stdlib.h"

//  定义 key
struct hash_map_str_key {
    uint32_t hash;
    char* key;
};
struct hash_map_str_key* hash_map_str_key_ref(struct hash_map_str_key* key, char* str);

//  定义一个实体
struct hash_map_str_int_entry {
    struct hash_node node;
    struct hash_map_str_key key;
    int val;
};

struct hash_map_str_int_entry* hash_map_str_int_entry_new(char* key, int val);
void hash_map_str_int_entry_del(struct hash_map_str_int_entry* entry);

struct hash_trait* hash_map_str_int_trait_init(struct hash_trait* trait);

#endif  //__hash_map_str_ptr_H_

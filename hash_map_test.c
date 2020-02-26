#include "hash_map.h"

#include <string.h>

#include "hash_map(str,int).h"
#include "upc_assert_message.h"

int main(int argc, char* argv[])
{
    //  再初 hash_map
    struct hash_trait trait = {0};
    struct hash_map h = {0};
    hash_map_init(&h, hash_map_str_int_trait_init(&trait), 97);

    struct hash_node* repl =
        hash_map_put(&h, (struct hash_node*)hash_map_str_int_entry_new("a", 1));
    ASSERT_MESSAGE((NULL == repl), "返回值为NULL");

    struct hash_map_str_key str_key;
    hash_map_str_key_ref(&str_key, "a");

    struct hash_map_str_int_entry* get_item =
        (struct hash_map_str_int_entry*)hash_map_get(&h, &str_key);
    ASSERT_MESSAGE((NULL != get_item), "返回值不能为NULL");
    ASSERT_MESSAGE((1 == get_item->val), "返回的 node 必须与添加的时候一致");
    ASSERT_MESSAGE((1 == hash_map_size(&h)), "个数必须正确");

    hash_map_put(&h, (struct hash_node*)hash_map_str_int_entry_new("b", 2));
    ASSERT_MESSAGE((2 == hash_map_size(&h)), "个数必须正确");

    hash_map_str_key_ref(&str_key, "b");
    get_item = (struct hash_map_str_int_entry*)hash_map_get(&h, &str_key);
    ASSERT_MESSAGE((NULL != get_item), "能够获取成功");
    ASSERT_MESSAGE((0 == strcmp(get_item->key.key, "b")), "key 值一样");
    ASSERT_MESSAGE((get_item->val == 2), "val 值一样");

    hash_map_str_key_ref(&str_key, "b");
    struct hash_map_str_int_entry* pop_entry =
        (struct hash_map_str_int_entry*)hash_map_pop(&h, &str_key);
    ASSERT_MESSAGE((NULL != pop_entry), "删除的时候，被删除的元素会被返回");
    ASSERT_MESSAGE((0 == strcmp(pop_entry->key.key, "b")), "key 值一样");
    ASSERT_MESSAGE((pop_entry->val == 2), "val 值一样");
    ASSERT_MESSAGE((1 == hash_map_size(&h)), "个数必须正确");

    pop_entry = (struct hash_map_str_int_entry*)hash_map_pop(&h, &str_key);
    ASSERT_MESSAGE((NULL == pop_entry),
                   "当元素已经被删除时，重新删除会返回NULL");
    ASSERT_MESSAGE((1 == hash_map_size(&h)), "个数必须正确");

    hash_map_str_key_ref(&str_key, "a");
    pop_entry = (struct hash_map_str_int_entry*)hash_map_pop(&h, &str_key);
    ASSERT_MESSAGE((NULL != pop_entry), "删除的时候，被删除的元素会被返回");
    ASSERT_MESSAGE((0 == strcmp(pop_entry->key.key, "a")), "key 值一样");
    ASSERT_MESSAGE((pop_entry->val == 1), "val 值一样");
    ASSERT_MESSAGE((0 == hash_map_size(&h)), "个数必须正确");

    hash_map_str_key_ref(&str_key, "c");
    (struct hash_map_str_int_entry*)hash_map_put(
        &h, (struct hash_node*)hash_map_str_int_entry_new("c", 3));
    struct hash_map_str_int_entry* exist_entry =
        (struct hash_map_str_int_entry*)hash_map_get(&h, &str_key);
    ASSERT_MESSAGE((NULL != exist_entry), "返回值为NULL");
    ASSERT_MESSAGE((exist_entry->val == 3), "val 值和 put 时的值一样");
    struct hash_map_str_int_entry* rep2 =
        (struct hash_map_str_int_entry*)hash_map_put(
            &h, (struct hash_node*)hash_map_str_int_entry_new("c", 33));
    ASSERT_MESSAGE((NULL != rep2), "返回值为NULL");
    ASSERT_MESSAGE((exist_entry == rep2), "");
    exist_entry = (struct hash_map_str_int_entry*)hash_map_get(&h, &str_key);
    ASSERT_MESSAGE((exist_entry != rep2), "新 entry 替换老entry");
    ASSERT_MESSAGE((exist_entry->val == 33), "val 被更换为新值");
    ASSERT_MESSAGE((1 == hash_map_size(&h)),
                   "同名的key只存在替换，所以数量应该不变");

    return 0;
}

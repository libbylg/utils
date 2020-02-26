#include "hash_map.h"
#include "hash_map(str,int).h"


#include "upc_assert_message.h"


#include <string.h>


int main(int argc, char* argv[])
{
    //  �ٳ� hash_map
    struct hash_trait trait = {0};
    struct hash_map h = {0};
    hash_map_init(&h, hash_map_str_int_trait_init(&trait), 97);

    struct hash_node* repl = hash_map_put(&h, (struct hash_node*)hash_map_str_int_entry_new("a", 1));
    ASSERT_MESSAGE((NULL == repl), "����ֵΪNULL");

    struct hash_map_str_key str_key;
    hash_map_str_key_ref(&str_key, "a");

    struct hash_map_str_int_entry* get_item = (struct hash_map_str_int_entry*)hash_map_get(&h, &str_key);
    ASSERT_MESSAGE((NULL != get_item), "����ֵ����ΪNULL");
    ASSERT_MESSAGE((1 == get_item->val), "���ص� node ��������ӵ�ʱ��һ��");
    ASSERT_MESSAGE((1 == hash_map_size(&h)), "����������ȷ");

    hash_map_put(&h, (struct hash_node*)hash_map_str_int_entry_new("b", 2));
    ASSERT_MESSAGE((2 == hash_map_size(&h)), "����������ȷ");

    hash_map_str_key_ref(&str_key, "b");
    get_item = (struct hash_map_str_int_entry*)hash_map_get(&h, &str_key);
    ASSERT_MESSAGE((NULL != get_item), "�ܹ���ȡ�ɹ�");
    ASSERT_MESSAGE((0 == strcmp(get_item->key.key, "b")), "key ֵһ��");
    ASSERT_MESSAGE((get_item->val == 2), "val ֵһ��");


    hash_map_str_key_ref(&str_key, "b");
    struct hash_map_str_int_entry* pop_entry = (struct hash_map_str_int_entry*)hash_map_pop(&h, &str_key);
    ASSERT_MESSAGE((NULL != pop_entry), "ɾ����ʱ�򣬱�ɾ����Ԫ�ػᱻ����");
    ASSERT_MESSAGE((0 == strcmp(pop_entry->key.key, "b")), "key ֵһ��");
    ASSERT_MESSAGE((pop_entry->val == 2), "val ֵһ��");
    ASSERT_MESSAGE((1 == hash_map_size(&h)), "����������ȷ");

    pop_entry = (struct hash_map_str_int_entry*)hash_map_pop(&h, &str_key);
    ASSERT_MESSAGE((NULL == pop_entry), "��Ԫ���Ѿ���ɾ��ʱ������ɾ���᷵��NULL");
    ASSERT_MESSAGE((1 == hash_map_size(&h)), "����������ȷ");

    hash_map_str_key_ref(&str_key, "a");
    pop_entry = (struct hash_map_str_int_entry*)hash_map_pop(&h, &str_key);
    ASSERT_MESSAGE((NULL != pop_entry), "ɾ����ʱ�򣬱�ɾ����Ԫ�ػᱻ����");
    ASSERT_MESSAGE((0 == strcmp(pop_entry->key.key, "a")), "key ֵһ��");
    ASSERT_MESSAGE((pop_entry->val == 1), "val ֵһ��");
    ASSERT_MESSAGE((0 == hash_map_size(&h)), "����������ȷ");

    hash_map_str_key_ref(&str_key, "c");
    (struct hash_map_str_int_entry*)hash_map_put(&h, (struct hash_node*)hash_map_str_int_entry_new("c", 3));
    struct hash_map_str_int_entry* exist_entry = (struct hash_map_str_int_entry*)hash_map_get(&h, &str_key);
    ASSERT_MESSAGE((NULL != exist_entry), "����ֵΪNULL");
    ASSERT_MESSAGE((exist_entry->val == 3), "val ֵ�� put ʱ��ֵһ��");
    struct hash_map_str_int_entry* rep2 = (struct hash_map_str_int_entry*)hash_map_put(&h, (struct hash_node*)hash_map_str_int_entry_new("c", 33));
    ASSERT_MESSAGE((NULL != rep2), "����ֵΪNULL");
    ASSERT_MESSAGE((exist_entry == rep2), "");
    exist_entry = (struct hash_map_str_int_entry*)hash_map_get(&h, &str_key);
    ASSERT_MESSAGE((exist_entry != rep2), "�� entry �滻��entry");
    ASSERT_MESSAGE((exist_entry->val == 33), "val ������Ϊ��ֵ");
    ASSERT_MESSAGE((1 == hash_map_size(&h)), "ͬ����keyֻ�����滻����������Ӧ�ò���");

    return 0;
}

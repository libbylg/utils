#ifndef list_head_H
#define list_head_H


struct list_head {
    struct list_head* next;
    struct list_head* prev;
};


static inline struct list_head* list_init(struct list_head* n)
{
    n->next = n;
    n->prev = n;
    return (n);
}


static inline struct list_head* list_insert(struct list_head* n, struct list_head* prev, struct list_head* next)
{
    n->next = next;
    n->prev = prev;
    prev->next = n;
    next->prev = n;
    return n;
}


static inline void list_remove(struct list_head* prev, struct list_head* next)
{
    next->prev = prev;
    prev->next = next;
}


static inline void list_add(struct list_head* n, struct list_head* pos)
{
    list_insert(n, pos, pos->next);
}


static inline void list_add_tail(struct list_head* n, struct list_head* pos)
{
    list_insert(n, n->prev, n);
}


static inline struct list_head* list_strip(struct list_head* n)
{
    list_remove(n->prev, n->next);
}


static inline void list_move(struct list_head* n, struct list_head* pos)
{
    list_strip(n);
    list_add(n, pos);
}


static inline void list_move_tail(struct list_head* n, struct list_head* pos)
{
    list_strip(n);
    list_add_tail(n, pos);
}


static inline void list_replace(struct list_head* old, struct list_head* n)
{
    n->next = old->next;
    n->next->prev = n;
    n->prev = old->prev;
    n->prev->next = n;
}


static inline void list_swap(struct list_head* entry1, struct list_head* entry2)
{
    struct list_head* pos = entry2->prev;

    list_strip(entry2);
    list_replace(entry1, entry2);
    if (pos == entry1) {
        pos = entry2;
    }
    list_add(entry1, pos);
}


static inline _Bool list_empty(struct list_head* n)
{
    return n->next == n;
}

static inline int list_is_first(const struct list_head* n, const struct list_head* lt)
{
    return n->prev == lt;
}

static inline int list_is_last(const struct list_head* n, const struct list_head* lt)
{
    return n->next == lt;
}

#define container_of(ptr, type, member)                                                                                \
    ({                                                                                                                 \
        　　const typeof(((type*)0)->member)* __mptr = (ptr);                                                          \
        　　(type*)((char*)__mptr - offsetof(type, member));                                                           \
    })


#ifndef prefetch
#define prefetch(x) __builtin_prefetch(x)
#endif


#define list_entry(ptr, type, member) ((type*)((char*)(ptr) - (size_t)(&((type*)0)->member)))

#define list_for_each(pos, head) for (pos = (head)->next; prefetch(pos->next), pos != (head); pos = pos->next)

#define list_for_each_safe(pos, n, head)                                                                               \
    for (pos = (head)->next, n = pos->next; prefetch(pos->next), pos != (head); pos = n, n = pos->next)

#endif //list_head_H

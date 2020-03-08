#ifndef BITARRAY_H
#define BITARRAY_H

#include <alloca.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "exports.h"
#include "macros.h"


// clang-format off
static uint64_t bit_array_masks[] = {
    0x0000000000000001,
    0x0000000000000002,
    0x0000000000000004,
    0x0000000000000008,
    0x0000000000000010,
    0x0000000000000020,
    0x0000000000000040,
    0x0000000000000080,
    0x0000000000000100,
    0x0000000000000200,
    0x0000000000000400,
    0x0000000000000800,
    0x0000000000001000,
    0x0000000000002000,
    0x0000000000004000,
    0x0000000000008000,
    0x0000000000010000,
    0x0000000000020000,
    0x0000000000040000,
    0x0000000000080000,
    0x0000000000100000,
    0x0000000000200000,
    0x0000000000400000,
    0x0000000000800000,
    0x0000000001000000,
    0x0000000002000000,
    0x0000000004000000,
    0x0000000008000000,
    0x0000000010000000,
    0x0000000020000000,
    0x0000000040000000,
    0x0000000080000000,
    0x0000000100000000,
    0x0000000200000000,
    0x0000000400000000,
    0x0000000800000000,
    0x0000001000000000,
    0x0000002000000000,
    0x0000004000000000,
    0x0000008000000000,
    0x0000010000000000,
    0x0000020000000000,
    0x0000040000000000,
    0x0000080000000000,
    0x0000100000000000,
    0x0000200000000000,
    0x0000400000000000,
    0x0000800000000000,
    0x0001000000000000,
    0x0002000000000000,
    0x0004000000000000,
    0x0008000000000000,
    0x0010000000000000,
    0x0020000000000000,
    0x0040000000000000,
    0x0080000000000000,
    0x0100000000000000,
    0x0200000000000000,
    0x0400000000000000,
    0x0800000000000000,
    0x1000000000000000,
    0x2000000000000000,
    0x4000000000000000,
    0x8000000000000000,
};
static_assert ((sizeof(bit_array_masks) / sizeof(bit_array_masks[0])) == (sizeof(uint64_t) * 8), "");
// clang-format on

static int align_with(int size, int width)
{
    return (size % width) ? (((size / width) + 1) * width) : size;
}

struct bit_array_t {
    int len;
    //
    uint64_t data[0];
};

static inline int bit_array_bytesof(int bits_cap)
{
    return sizeof(struct bit_array_t) + (align_with(bits_cap, sizeof(uint64_t) * 8) / (sizeof(uint8_t) * 8));
}

static inline struct bit_array_t* bit_array_init(struct bit_array_t* v, int bits_cap)
{
    if (NULL != v) {
        v->len = align_with(bits_cap, sizeof(uint64_t) * 8) / (sizeof(uint64_t) * 8);
    }
    return v;
}

//static inline struct bit_array_t* bit_array_malloc(int bits_cap)
#define bit_array_malloc(bits_cap) (bit_array_init((struct bit_array_t*)malloc(bit_array_memsize(bits_cap)), bits_cap))


//static inline struct bit_array_t* bit_array_aclloca(int bit_cap)
#define bit_array_alloca(bits_cap) (bit_array_init((struct bit_array_t*)alloca(bit_array_memsize(bits_cap)), bits_cap))

static inline void bit_array_free(struct bit_array_t* v)
{
    free(v);
}


//static inline int bit_array_cap(struct bit_array_t* v);
#define bit_array_cap(v) (((v)->len) * (sizeof(uint64_t) * 8))


//static inline struct bit_array_t* bit_array_reset(struct bit_array_t* v, [int def]);
#define bit_array_reset(...) _MACRO_MAP2(__VA_ARGS__, _bit_array_reset2, _bit_array_reset1)(__VA_ARGS__)

#define _bit_array_reset1(v)                                                                                           \
    ({                                                                                                                 \
        memset((v)->data, 0, (sizeof(uint64_t) * (v)->len));                                                           \
        (v);                                                                                                           \
    })

#define _bit_array_reset2(v, fill)                                                                                     \
    ({                                                                                                                 \
        memset((v)->data, ((0 == fill) ? 0x00 : 0xFF), (sizeof(uint64_t) * (v)->cap));                                 \
        (v);                                                                                                           \
    })


static inline int bit_array_get(struct bit_array_t* v, int index)
{
    assert(index >= 0);
    int blkIndex = index / (sizeof(uint64_t) * 8);
    int bitIndex = index % (sizeof(uint64_t) * 8);
    return v->data[blkIndex] & bit_array_masks[bitIndex];
}

static inline void bit_array_set(struct bit_array_t* v, int index)
{
    assert(index >= 0);
    int blkIndex = index / (sizeof(uint64_t) * 8);
    int bitIndex = index % (sizeof(uint64_t) * 8);
    v->data[blkIndex] |= bit_array_masks[bitIndex];
}

static inline void bit_array_clr(struct bit_array_t* v, int index)
{
    assert(index >= 0);
    int blkIndex = index / (sizeof(uint64_t) * 8);
    int bitIndex = index % (sizeof(uint64_t) * 8);
    v->data[blkIndex] &= (~bit_array_masks[bitIndex]);
}

static inline int bit_array_count_of(struct bit_array_t* v, int val)
{
    static_assert(sizeof(unsigned long long) == sizeof(uint64_t), "`__builtin_popcountll` need `unsigned long long`");
    int count = 0;
    for (int i = 0; i < v->len; i++) {
        count += __builtin_popcountll(v->data[i]);
    }

    if (0 == val) {
        return bit_array_cap(v) - count;
    }

    return count;
}

#endif //BITARRAY_H

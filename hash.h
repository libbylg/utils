#ifndef __hash_func_int_H_
#define __hash_func_int_H_


#include "inttype.h"


/* 2^31 + 2^29 - 2^25 + 2^22 - 2^19 - 2^16 + 1 */
#define GOLDEN_RATIO_PRIME_32 0x9e370001UL
/*  2^63 + 2^61 - 2^57 + 2^54 - 2^51 - 2^18 + 1 */
#define GOLDEN_RATIO_PRIME_64 0x9e37fffffffc0001UL



static inline  uint64 hash_64(uint64 val, unsigned int bits)
{
    uint64 hash = val;

    /*  Sigh, gcc can't optimise this alone like it does for 32 bits. */
    uint64 n = hash;
    n <<= 18;
    hash -= n;
    n <<= 33;
    hash -= n;
    n <<= 3;
    hash += n;
    n <<= 3;
    hash -= n;
    n <<= 4;
    hash += n;
    n <<= 2;
    hash += n;

    /* High bits are more random, so use them. */
    return hash >> (64 - bits);
}

static inline  uint32 hash_32(uint32 val, unsigned int bits)
{
    /* On some cpus multiply is faster, on others gcc will do shifts */
    uint32 hash = val * GOLDEN_RATIO_PRIME_32;

    /* High bits are more random, so use them. */
    return hash >> (32 - bits);
}


#define hash_long(val, bits) ((sizeof(long) == 8)?hash_64(val, bits):hash_32(val, bits))

#define hash_ptr(ptr, bits) hash_long((unsigned long)ptr, bits)

STATIC_ASSERT((sizeof(long) == sizeof(void*)), "字符串长度必须等于 long 类型的长度");

// DJB2
// DJB2a (variant using xor rather than +)
// FNV-1 (32-bit)
// FNV-1a (32-bit)
// SDBM
// CRC32
// Murmur2 (32-bit)
// Murmur3
// SuperFastHash xx
// LoseLose

static inline uint32 SDBMHash(char *str)
{
    uint32 hash = 0;

    while (*str)
    {
        // equivalent to: hash = 65599*hash + (*str++);
        hash = (*str++) + (hash << 6) + (hash << 16) - hash;
    }

    return (hash & 0x7FFFFFFF);
}

// RS Hash Function
static inline uint32 RSHash(char *str)
{
    uint32 b = 378551;
    uint32 a = 63689;
    uint32 hash = 0;

    while (*str)
    {
        hash = hash * a + (*str++);
        a *= b;
    }

    return (hash & 0x7FFFFFFF);
}

// JS Hash Function
static inline uint32 JSHash(char *str)
{
    uint32 hash = 1315423911;

    while (*str)
    {
        hash ^= ((hash << 5) + (*str++) + (hash >> 2));
    }

    return (hash & 0x7FFFFFFF);
}

// P. J. Weinberger Hash Function
static inline uint32 PJWHash(char *str)
{
    uint32 BitsInUnignedInt = (uint32)(sizeof(uint32) * 8);
    uint32 ThreeQuarters    = (uint32)((BitsInUnignedInt  * 3) / 4);
    uint32 OneEighth        = (uint32)(BitsInUnignedInt / 8);
    uint32 HighBits         = (uint32)(0xFFFFFFFF) << (BitsInUnignedInt - OneEighth);
    uint32 hash             = 0;
    uint32 test             = 0;

    while (*str)
    {
        hash = (hash << OneEighth) + (*str++);
        if ((test = hash & HighBits) != 0)
        {
            hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }

    return (hash & 0x7FFFFFFF);
}

// ELF Hash Function
static inline uint32 ELFHash(char *str)
{
    uint32 hash = 0;
    uint32 x    = 0;

    while (*str)
    {
        hash = (hash << 4) + (*str++);
        if ((x = hash & 0xF0000000L) != 0)
        {
            hash ^= (x >> 24);
            hash &= ~x;
        }
    }

    return (hash & 0x7FFFFFFF);
}

// BKDR Hash Function
static inline uint32 BKDRHash(char *str)
{
    uint32 seed = 131; // 31 131 1313 13131 131313 etc..
    uint32 hash = 0;

    while (*str)
    {
        hash = hash * seed + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}

// DJB Hash Function
static inline uint32 DJBHash(char *str)
{
    uint32 hash = 5381;

    while (*str)
    {
        hash += (hash << 5) + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}

static inline uint32 DJB2Hash(char *str)
{
    unsigned long hash = 5381;
    while (*str)
    {
        hash = ((hash << 5) + hash) + (uint32)*(str++); /* hash * 33 + c */
    }

    return (uint32)(hash & 0x7FFFFFFF);
}

// AP Hash Function
static inline uint32 APHash(char *str)
{
    uint32 hash = 0;
    for ( int i = 0; *str; i++)
    {
        if ((i & 1) == 0)
        {
            hash ^= ((hash << 7) ^ (*str++) ^ (hash >> 3));
        }
        else
        {
            hash ^= (~((hash << 11) ^ (*str++) ^ (hash >> 5)));
        }
    }

    return (hash & 0x7FFFFFFF);
}

static inline uint32 LoseLoseHash(char *str)
{
    uint32 hash = 0;
    while (*str)
    {
        hash += *(str++);
    }

    return hash;
}


static inline uint32 FNV_hash(char* str)
{
    unsigned long h = 2166136261UL;

    while (*str)
    {
        h = (h * 16777619) ^ *str ;
    }

    return (uint32)(h & 0x7FFFFFFF);
}

static inline uint32 FNV1a_hash(char* str)
{
    unsigned long h = 2166136261UL;

    while (*str)
    {
        h = (h ^ *str) * 16777619;
    }

    return (uint32)(h & 0x7FFFFFFF);
}


static inline uint32    murmurhash (const char *key, uint32 len, uint32 seed) 
{
        uint32 c1 = 0xcc9e2d51;
        uint32 c2 = 0x1b873593;
        uint32 r1 = 15;
        uint32 r2 = 13;
        uint32 m = 5;
        uint32 n = 0xe6546b64;
        uint32 h = 0;
        uint32 k = 0;
        uint8 *d = (uint8 *) key; // 32 bit extract from `key'
        uint32 *chunks = NULL;
        uint8 *tail = NULL; // tail - last 8 bytes
        int i = 0;
        int l = len / 4; // chunk length

        h = seed;

        chunks = (uint32 *) (d + l * 4); // body
        tail = (uint8 *) (d + l * 4); // last 8 byte chunk of `key'

        // for each 4 byte chunk of `key'
        for (i = -l; i != 0; ++i) {
            // next 4 byte chunk of `key'
            k = chunks[i];

            // encode next 4 byte chunk of `key'
            k *= c1;
            k = (k << r1) | (k >> (32 - r1));
            k *= c2;

            // append to hash
            h ^= k;
            h = (h << r2) | (h >> (32 - r2));
            h = h * m + n;
        }

        k = 0;

        // remainder
        switch (len & 3) { // `len % 4'
        case 3: k ^= (tail[2] << 16);
        case 2: k ^= (tail[1] << 8);

        case 1:
            k ^= tail[0];
            k *= c1;
            k = (k << r1) | (k >> (32 - r1));
            k *= c2;
            h ^= k;
        }

        h ^= len;

        h ^= (h >> 16);
        h *= 0x85ebca6b;
        h ^= (h >> 13);
        h *= 0xc2b2ae35;
        h ^= (h >> 16);

        return h;
}

#endif//__hash_func_int_H_

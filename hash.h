#ifndef __hash_func_int_H_
#define __hash_func_int_H_


#include <stdint.h>


/* 2^31 + 2^29 - 2^25 + 2^22 - 2^19 - 2^16 + 1 */
#define GOLDEN_RATIO_PRIME_32 0x9e370001UL
/*  2^63 + 2^61 - 2^57 + 2^54 - 2^51 - 2^18 + 1 */
#define GOLDEN_RATIO_PRIME_64 0x9e37fffffffc0001UL


static inline uint64_t hash_64(uint64_t val, unsigned int bits)
{
    uint64_t hash = val;

    /*  Sigh, gcc can't optimise this alone like it does for 32 bits. */
    uint64_t n = hash;
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

static inline uint32_t hash_32(uint32_t val, unsigned int bits)
{
    /* On some cpus multiply is faster, on others gcc will do shifts */
    uint32_t hash = val * GOLDEN_RATIO_PRIME_32;

    /* High bits are more random, so use them. */
    return hash >> (32 - bits);
}

#define hash_long(val, bits) ((sizeof(long) == 8) ? hash_64((uint64_t)ptr, bits) : hash_32((uint32_t)ptr, bits))
#define hash_ptr(ptr, bits) ((sizeof(void *) == 8) ? hash_64((uint64_t)ptr, bits) : hash_32((uint32_t)ptr, bits))


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

static inline uint32_t SDBMHash(char *str)
{
    uint32_t hash = 0;

    while (*str) {
        // equivalent to: hash = 65599*hash + (*str++);
        hash = (*str++) + (hash << 6) + (hash << 16) - hash;
    }

    return (hash & 0x7FFFFFFF);
}

// RS Hash Function
static inline uint32_t RSHash(char *str)
{
    uint32_t b = 378551;
    uint32_t a = 63689;
    uint32_t hash = 0;

    while (*str) {
        hash = hash * a + (*str++);
        a *= b;
    }

    return (hash & 0x7FFFFFFF);
}

// JS Hash Function
static inline uint32_t JSHash(char *str)
{
    uint32_t hash = 1315423911;

    while (*str) {
        hash ^= ((hash << 5) + (*str++) + (hash >> 2));
    }

    return (hash & 0x7FFFFFFF);
}

// P. J. Weinberger Hash Function
static inline uint32_t PJWHash(char *str)
{
    uint32_t BitsInUnignedInt = (uint32_t)(sizeof(uint32_t) * 8);
    uint32_t ThreeQuarters = (uint32_t)((BitsInUnignedInt * 3) / 4);
    uint32_t OneEighth = (uint32_t)(BitsInUnignedInt / 8);
    uint32_t HighBits = (uint32_t)(0xFFFFFFFF) << (BitsInUnignedInt - OneEighth);
    uint32_t hash = 0;
    uint32_t test = 0;

    while (*str) {
        hash = (hash << OneEighth) + (*str++);
        if ((test = hash & HighBits) != 0) {
            hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }

    return (hash & 0x7FFFFFFF);
}

// ELF Hash Function
static inline uint32_t ELFHash(char *str)
{
    uint32_t hash = 0;
    uint32_t x = 0;

    while (*str) {
        hash = (hash << 4) + (*str++);
        if ((x = hash & 0xF0000000L) != 0) {
            hash ^= (x >> 24);
            hash &= ~x;
        }
    }

    return (hash & 0x7FFFFFFF);
}

// BKDR Hash Function
static inline uint32_t BKDRHash(char *str)
{
    uint32_t seed = 131; // 31 131 1313 13131 131313 etc..
    uint32_t hash = 0;

    while (*str) {
        hash = hash * seed + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}

// DJB Hash Function
static inline uint32_t DJBHash(char *str)
{
    uint32_t hash = 5381;

    while (*str) {
        hash += (hash << 5) + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}

static inline uint32_t DJB2Hash(char *str)
{
    unsigned long hash = 5381;
    while (*str) {
        hash = ((hash << 5) + hash) + (uint32_t) * (str++); /* hash * 33 + c */
    }

    return (uint32_t)(hash & 0x7FFFFFFF);
}

// AP Hash Function
static inline uint32_t APHash(char *str)
{
    uint32_t hash = 0;
    for (int i = 0; *str; i++) {
        if ((i & 1) == 0) {
            hash ^= ((hash << 7) ^ (*str++) ^ (hash >> 3));
        } else {
            hash ^= (~((hash << 11) ^ (*str++) ^ (hash >> 5)));
        }
    }

    return (hash & 0x7FFFFFFF);
}

static inline uint32_t LoseLoseHash(char *str)
{
    uint32_t hash = 0;
    while (*str) {
        hash += *(str++);
    }

    return hash;
}


static inline uint32_t FNV_hash(char *str)
{
    unsigned long h = 2166136261UL;

    while (*str) {
        h = (h * 16777619) ^ *str;
    }

    return (uint32_t)(h & 0x7FFFFFFF);
}

static inline uint32_t FNV1a_hash(char *str)
{
    unsigned long h = 2166136261UL;

    while (*str) {
        h = (h ^ *str) * 16777619;
    }

    return (uint32_t)(h & 0x7FFFFFFF);
}


static inline uint32_t murmurhash(const char *key, uint32_t len, uint32_t seed)
{
    uint32_t c1 = 0xcc9e2d51;
    uint32_t c2 = 0x1b873593;
    uint32_t r1 = 15;
    uint32_t r2 = 13;
    uint32_t m = 5;
    uint32_t n = 0xe6546b64;
    uint32_t h = 0;
    uint32_t k = 0;
    uint8_t *d = (uint8_t *)key; // 32 bit extract from `key'
    uint32_t *chunks = NULL;
    uint8_t *tail = NULL; // tail - last 8 bytes
    int i = 0;
    int l = len / 4;  // chunk length

    h = seed;

    chunks = (uint32_t *)(d + l * 4); // body
    tail = (uint8_t *)(d + l * 4);    // last 8 byte chunk of `key'

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
    switch (len & 3) {  // `len % 4'
        case 3:
            k ^= (tail[2] << 16);
        case 2:
            k ^= (tail[1] << 8);

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

#endif  //__hash_func_int_H_

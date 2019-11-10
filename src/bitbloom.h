#if !defined(BIT_BLOOM_FILTER_H)
#define BIT_BLOOM_FILTER_H

#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <xxhash.h>
#include <math.h>

#include "bitagg.h"

// :: Type -> size_t -> Type
#define bitbloom(type) \
    struct { \
        size_t num_hashes, bits; \
        decl_bitbuffer(type, data); \
    }

#define bitbloom_optimal_m(_expected, _acceptable) \
    llround((-(_expected)) * log(_acceptable) / (log(2) * log(2)))

#define bitbloom_optimal_k(_expected, _m) \
    llround(((_m) / (_expected)) * log(2))

#define bitbloom_optimal_init(_type, _expected, _acceptable) \
    { \
        .bits = bitbloom_optimal_m((_expected), (_acceptable)), \
        .num_hashes = bitbloom_optimal_k((_expected), bitbloom_optimal_m((_expected), (_acceptable))), \
        .data = init_bitbuffer(_type, bitbloom_optimal_m((_expected), (_acceptable))) \
    }

// :: bitbloom(Type, size_t) -> const char * -> size_t -> ()
#define bloom_insert(_blum, _data, _len) \
    do { \
        uint64_t r = XXH64(((char * )(_data)), (_len), 0); \
        uint32_t top = r & ~((~0UL) >> 32) >> 32; \
        uint32_t bot = r & (~0UL) >> 32; \
        for ( size_t i = (_blum).num_hashes; i; setbit((_blum).data, (top + (i - 1) * bot) % (_blum).bits), -- i ); \
    } while ( false )

// :: bitbloom(Type, size_t) -> const char * -> size_t -> out bool -> ()
#define bloom_elem(_blum, _data, _len, _res) \
    do { \
        uint64_t r = XXH64(((char * )(_data)), (_len), 0); \
        uint32_t top = r & ~((~0UL) >> 32) >> 32; \
        uint32_t bot = r & (~0UL) >> 32; \
        _res = true; \
        for ( size_t i = (_blum).num_hashes; i; _res &= getbit((_blum).data, (top + (i - 1) * bot) % (_blum).bits), -- i ); \
    } while ( false )

// broken
// bitbloom(Type, size_t) -> out double -> ()
#define bloom_approximate_count(_blum, _res) \
    do { \
        size_t _popcount = 0; \
        bitpopcount((_blum).data, (_blum).bits, _popcount); \
        (_res) = (((double )(-(_blum).bits)) / (_blum).num_hashes) * log(1 - (_popcount / ((double )(_blum).bits))); \
    } while ( false )

#endif // BIT_BLOOM_FILTER_H

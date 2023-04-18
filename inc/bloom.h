#ifndef BITAGG_BLOOM_H
#define BITAGG_BLOOM_H

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <xxhash.h>
#include <math.h>

#include <common.h>
#include <vector.h>

// :: Type -> size_t -> Type
#define bitbloom(_type) \
    struct { \
        size_t num_hashes, bits; \
        bitbuffer(_type, data); \
    }

// :: size_t -> double -> size_t
#define bitbloom_optimal_m(_expected, _acceptable) \
    ((size_t )(ceil((-(_expected)) * log(_acceptable) / (log(2) * log(2)))))

// :: size_t -> double -> size_t
#define bitbloom_optimal_k(_expected, _m) \
    ((size_t )(ceil(((_m) / (_expected)) * log(2))))

// :: Type -> size_t -> double -> bitbloom(Type)
#define bitbloom_optimal_init(_type, _expected, _acceptable) \
    { \
        .bits = bitbloom_optimal_m((_expected), (_acceptable)), \
        .num_hashes = bitbloom_optimal_k((_expected), bitbloom_optimal_m((_expected), (_acceptable))), \
        .data = bitbuffer_init(_type, bitbloom_optimal_m((_expected), (_acceptable))) \
    }

// :: bitbloom(Type) -> ()
#define bitbloom_free(_blum) \
    do { \
        if ( (_blum).data ) { free((_blum).data); } \
    } while ( false )

// :: bitbloom(Type) -> const char * -> size_t -> ()
#define bloom_insert(_blum, _data, _len) \
    do { \
        uint64_t r = XXH64(((char * )(_data)), (_len), 0); \
        uint32_t top = r & ~((~0UL) >> 32) >> 32; \
        uint32_t bot = r & (~0UL) >> 32; \
        for ( size_t i = (_blum).num_hashes; i; usetbit((_blum).data, (top + (i - 1) * bot) % (_blum).bits), -- i ); \
    } while ( false )

// bitbloom(Type) -> out double -> ()
#define bloom_approximate_count(_blum, _res) \
    do { \
        size_t _popcount = 0; \
        ubitpopcount((_blum).data, (_blum).bits, _popcount); \
        (_res) = ((0.0-(_blum).bits) / ((double )(_blum).num_hashes)) * log(1.0 - ((_popcount) / ((double )(_blum).bits))); \
    } while ( false )

// bitbloom(Type) -> out double -> ()
#define bloom_approximate_fp_rate(_blum, _res) \
    do { \
        double _nstar = 0; \
        bloom_approximate_count((_blum), _nstar); \
        (_res) = pow(1.0 - exp(-((double )(_blum).num_hashes) * _nstar / ((_blum).bits)), ((_blum).num_hashes)); \
    } while ( false )

// :: bitbloom(Type) -> const char * -> size_t -> out double -> ()
#define bloom_elem(_blum, _data, _len, _res) \
    do { \
        uint64_t r = XXH64(((char * )(_data)), (_len), 0); \
        uint32_t top = r & ~((~0UL) >> 32) >> 32; \
        uint32_t bot = r & (~0UL) >> 32; \
        bool found = true; \
        for ( size_t i = (_blum).num_hashes; i; found &= ugetbit((_blum).data, (top + (i - 1) * bot) % (_blum).bits), -- i ); \
        double fp_rate = 0; \
        bloom_approximate_fp_rate(_blum, fp_rate); \
        (_res) = found ? (1 - fp_rate) : 0; \
    } while ( false )

#endif

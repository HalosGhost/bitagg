#if !defined(BIT_AGGREGATE_H)
#define BIT_AGGREGATE_H

#pragma once

#include <limits.h>
#include <stdbool.h>

// :: a -> size_t
#define bitsof(x) (sizeof(x) * CHAR_BIT)
// :: T -> size_t -> size_t
#define members_needed(type, length) (((length) + bitsof(type) - 1) / bitsof(type))
// :: T -> size_t -> size_t
#define bytes_needed(type, length) (members_needed(type, length) * sizeof(type))

#define decl_bitarray(type, name, length) type name [members_needed(type, (length))]
#define init_bitarray() { 0 }

#define decl_bitbuffer(type, name) type * name
#define init_bitbuffer(type, length) calloc(members_needed(type, (length)), sizeof(type))

#define bitarray(type, name, length) decl_bitarray(type, name, length) = init_bitarray()
#define bitbuffer(type, name, length) decl_bitbuffer(type, name) = init_bitbuffer(type, length)

// macro-hygiene helpers
#define CAT(a, b) a ## b
#define CONCAT(a, b) CAT(a, b)
#define UNIQUE() __LINE__
#define VAR(name) CONCAT(name, UNIQUE())

#define voidbitop(type, arr, idx, op) do { \
    type * VAR(a) = (arr); \
    size_t VAR(i) = (idx); \
    VAR(a)[VAR(i) / bitsof(*VAR(a))] op (1ull << (VAR(i) % bitsof(*VAR(a)))); \
} while ( false )

// :: Type -> [a] -> size_t -> out bool -> ()
#define getbit(type, arr, idx, res) do { \
    type * VAR(a) = (arr); \
    size_t VAR(i) = (idx); \
    (res) = !!(VAR(a)[VAR(i) / bitsof(*VAR(a))] & (1ull << (VAR(i) % bitsof(*VAR(a))))); \
} while ( false )

// :: Type -> [a] -> size_t -> ()
#define setbit(type, arr, idx)    voidbitop(type, arr, idx, |=  )
#define clearbit(type, arr, idx)  voidbitop(type, arr, idx, &= ~)
#define togglebit(type, arr, idx) voidbitop(type, arr, idx, ^=  )

// :: Type -> [a] -> size_t -> bool -> ()
#define assignbit(type, arr, idx, val) do { \
    type * VAR(a) = (arr); \
    size_t VAR(i) = (idx); \
    if ((val)) { \
        setbit(type, VAR(a), VAR(i)); \
    } else { \
        clearbit(type, VAR(a), VAR(i)); \
    } \
} while ( false )

// :: Type -> [a] -> size_t -> out size_t -> ()
#define bitpopcount(type, arr, size, res) \
    do { for ( size_t i = 0; i < (size); ++i ) { \
        getbit(type, arr, i, res); \
    } } while ( false )

/**
 * Note: All macros below this block are unhygenic
 **
 * Because all of these macros evaluate at least one argument twice, passing
 * them any side-effecting expression is unsafe.
 *
 * There are two benefits to their use:
 * 1. They are (mostly) expressions rather than statements/blocks
 * 2. They do not require the underlying type to be passed in
 */

#define primbitop(arr, idx, op) ((arr)[(idx) / bitsof(*(arr))] op (1ull << ((idx) % bitsof(*(arr)))))

// :: [a] -> size_t -> a
#define ugetbit(arr, idx)    (!!primbitop((arr), (idx), &   ))
#define usetbit(arr, idx)       primbitop((arr), (idx), |=  )
#define uclearbit(arr, idx)     primbitop((arr), (idx), &= ~)
#define utogglebit(arr, idx)    primbitop((arr), (idx), ^=  )

// :: [a] -> size_t -> bool -> a
#define uassignbit(arr, idx, val) \
    ((val) ? usetbit((arr), (idx)) : uclearbit((arr), (idx)))

// :: [a] -> size_t -> out size_t -> ()
#define ubitpopcount(arr, size, res) \
    do { for ( size_t i = 0; i < (size); ++ i ) { \
        (res) += ugetbit(arr, i); \
    } } while ( false )

#endif // BIT_AGGREGATE_H


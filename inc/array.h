#ifndef BITAGG_ARRAY_H
#define BITAGG_ARRAY_H

#pragma once

#include <common.h>

#define bitarray(_type, _name, _length) _type _name [members_needed(_type, (_length))]
#define bitarray_init() { 0 }

// :: Type -> [a] -> size_t -> bitwise_oeprator -> ()
#define voidbitop(_type, _arr, _idx, _op) do { \
    _type * VAR(a) = (_arr); \
    size_t VAR(i) = (_idx); \
    VAR(a)[VAR(i) / bitsof(*VAR(a))] op (1ull << (VAR(i) % bitsof(*VAR(a)))); \
} while ( false )

// :: Type -> [a] -> size_t -> out bool -> ()
#define getbit(_type, _arr, _idx, _res) do { \
    _type * VAR(a) = (_arr); \
    size_t VAR(i) = (_idx); \
    (_res) = !!(VAR(a)[VAR(i) / bitsof(*VAR(a))] & (1ull << (VAR(i) % bitsof(*VAR(a))))); \
} while ( false )

// :: Type -> [a] -> size_t -> ()
#define setbit(_type, _arr, _idx)    voidbitop(_type, _arr, _idx, |=  )
#define clearbit(_type, _arr, _idx)  voidbitop(_type, _arr, _idx, &= ~)
#define togglebit(_type, _arr, _idx) voidbitop(_type, _arr, _idx, ^=  )

// :: Type -> [a] -> size_t -> bool -> ()
#define assignbit(_type, _arr, _idx, _val) do { \
    _type * VAR(a) = (_arr); \
    size_t VAR(i) = (_idx); \
    if ((_val)) { \
        setbit(_type, VAR(a), VAR(i)); \
    } else { \
        clearbit(_type, VAR(a), VAR(i)); \
    } \
} while ( false )

// :: Type -> [a] -> size_t -> out size_t -> ()
#define bitpopcount(_type, _arr, _size, _res) \
    do { for ( size_t i = 0; i < (_size); ++i ) { \
        getbit(_type, _arr, i, _res); \
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

#define ubitop(_arr, _idx, _op) \
    ((_arr)[(_idx) / bitsof(*(_arr))] _op (1ull << ((_idx) % bitsof(*(_arr)))))

// :: [a] -> size_t -> a
#define ugetbit(_arr, _idx)    (!!ubitop((_arr), (_idx), &   ))
#define usetbit(_arr, _idx)       ubitop((_arr), (_idx), |=  )
#define uclearbit(_arr, _idx)     ubitop((_arr), (_idx), &= ~)
#define utogglebit(_arr, _idx)    ubitop((_arr), (_idx), ^=  )

// :: [a] -> size_t -> bool -> a
#define uassignbit(_arr, _idx, _val) \
    ((_val) ? usetbit((_arr), (_idx)) : uclearbit((_arr), (_idx)))

// :: [a] -> size_t -> out size_t -> ()
#define ubitpopcount(_arr, _size, _res) \
    do { for ( size_t i = 0; i < (_size); ++ i ) { \
        (_res) += ugetbit(_arr, i); \
    } } while ( false )

#endif


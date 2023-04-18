#ifndef BITAGG_COMMON_H
#define BITAGG_COMMON_H

#pragma once

#include <limits.h>
#include <stdbool.h>

// :: a -> size_t
#define bitsof(_x) (sizeof(_x) * CHAR_BIT)
// :: Type -> size_t -> size_t
#define members_needed(_type, _length) (((_length) + bitsof(_type) - 1) / bitsof(_type))
// :: Type -> size_t -> size_t
#define bytes_needed(_type, _length) (members_needed(_type, _length) * sizeof(_type))
// :: Type -> size_t -> size_t
#define bits_needed(_type, _length) (members_needed(_type, _length) * bitsof(_type))

// macro-hygiene helpers
#define CAT(_a, _b) _a ## _b
#define CONCAT(_a, _b) CAT(_a, _b)
#define UNIQUE() __LINE__
#define VAR(_name) CONCAT(_name, UNIQUE())

#endif


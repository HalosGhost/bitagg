#ifndef BITAGG_VECTOR_H
#define BITAGG_VECTOR_H

#pragma once

#include <common.h>
#include <array.h>

#define bitbuffer(_type, _name) _type * _name
#define bitbuffer_init(_type, _length) calloc(members_needed(_type, (_length)), sizeof(_type))

#endif


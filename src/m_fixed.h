// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//    Fixed point arithemtics, implementation.
//
//-----------------------------------------------------------------------------


#ifndef __M_FIXED__
#define __M_FIXED__

#include <inttypes.h>
#include <limits.h>

//
// Fixed point, 32bit as 16.16.
//
#define FRACBITS        16
#define FRACUNIT        (1<<FRACBITS)

typedef int32_t fixed_t;

static inline int D_abs(fixed_t x)
{
    fixed_t _s = x >> 31;
    return (x ^ _s) - _s;
}

static inline fixed_t __attribute__((always_inline)) FixedMul(fixed_t a, fixed_t b)
{
    return (fixed_t) (((int64_t) a * (int64_t) b) >> FRACBITS);
}

static inline fixed_t __attribute__((always_inline)) FixedDiv(fixed_t a, fixed_t b)
{
    if ((D_abs(a) >> 14) >= D_abs(b))
    {
	    return (a^b) < 0 ? INT_MIN : INT_MAX;
    }
    else
    {
        int64_t result;

        result = ((int64_t) a << FRACBITS) / b;

        return (fixed_t) result;
    }
}

#endif

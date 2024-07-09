// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// DESCRIPTION:
//	Lookup tables.
//	Do not try to look them up :-).
//	In the order of appearance: 
//
//	int finetangent[4096]	- Tangens LUT.
//	 Should work with BAM fairly well (12 of 16bit,
//      effectively, by shifting).
//
//	int finesine(10240)		- Sine lookup.
//	 Guess what, serves as cosine, too.
//	 Remarkable thing is, how to use BAMs with this? 
//
//	int tantoangle[2049]	- ArcTan LUT,
//	  maps tan(angle) to angle fast. Gotta search.	
//    
//-----------------------------------------------------------------------------


#ifndef __TABLES__
#define __TABLES__

// include libdragon header for fast math
#include <libdragon.h>
#define PI				3.141592657f

#include "m_fixed.h"
	
#define FINEANGLES		8192
#define FINEMASK		(FINEANGLES-1)


// 0x100000000 to 0x2000
#define ANGLETOFINESHIFT	19		

// Binary Angle Measument, BAM.
#define ANG45		0x20000000
#define ANG90		0x40000000
#define ANG180		0x80000000
#define ANG270		0xc0000000

#define SLOPERANGE		2048
#define SLOPEBITS		11
#define DBITS			(FRACBITS-SLOPEBITS)

typedef unsigned angle_t;

static inline int32_t  __attribute__((always_inline)) finesine(int32_t x)
{
    return (int32_t) (FRACUNIT * fm_sinf((x + 0.5f) * PI * 2 / FINEANGLES));
}

static inline int32_t __attribute__((always_inline)) finecosine(int32_t x)
{
    return finesine(x + 2048);
}

static inline int32_t  __attribute__((always_inline)) finetangentf(int32_t x)
{
    return (int32_t) (FRACUNIT * tanf((x - (FINEANGLES / 4) + 0.5f) * PI * 2 / FINEANGLES));
}

#endif

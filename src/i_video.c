// Emacs style mode select   -*- C++ -*- h
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
// $Log:$
//
// DESCRIPTION:
//	DOOM graphics stuff for Nintendo 64, libdragon
//
//-----------------------------------------------------------------------------

#include <libdragon.h>
#include <stdlib.h>
#include <math.h>

#include "doomstat.h"
#include "i_system.h"
#include "v_video.h"
#include "m_argv.h"
#include "d_main.h"

#include "doomdef.h"

// globals

// main display
surface_t* _dc;
// frame buffer that accumulates everything drawn by CPU before we blit to screen with RDP
// TODO: Get rid of this when we draw the entire frame using RDP
surface_t frame_buffer;

// use this to hold _dc->buffer pointer whenever we get a surface in D_DoomLoop
// now each R_DrawColumn/R_DrawSpan call only needs one "lw" instruction to get screen
// instead of two
// saves (num cols + num spans) "lw" instructions per rendered frame
// that is 1000+ loads per frame
void* bufptr;

void I_StartFrame(void)
{
    rdpq_attach(display_get(), NULL);
}

void I_ShutdownGraphics(void)
{
}


void I_UpdateNoBlit(void)
{
}

void I_FinishUpdate(void)
{
    rdpq_set_mode_standard();
    rdpq_tex_blit(_dc, 0, 0, NULL);

#ifndef NO_FPS_COUNTER
    rdpq_text_printf(NULL, FONT_BUILTIN_DEBUG_MONO, 16, 20, "FPS: %f", display_get_fps());
#endif

    rdpq_detach_show();
}

//
// I_ReadScreen
//
void I_ReadScreen(uint16_t* screen)
{
    memcpy(screen, bufptr, 320*200*2);
}

//
// Palette stuff.
//

void I_ForcePaletteUpdate(void)
{
}


//
// I_SetPalette
//
// this is intentionally uint32_t
// graphics_make_color will pack two 16-bit colors into a 32-bit word
// and we use that full word when rendering columns and spans in low-detail mode
// the color lookup is a single index into palarray
// no shifting and masking necessary to create a doubled pixel
// and it gets written to the 16-bit framebuffer as two pixels with a single 32-bit write
uint32_t*  palarray;
static uint32_t __attribute__((aligned(8))) default_palarray[256];
static uint32_t __attribute__((aligned(8))) current_palarray[256];
static uint32_t *old_palarray = NULL;

void I_SavePalette(void)
{
    old_palarray = current_palarray;
    palarray = default_palarray;
}

void I_RestorePalette(void)
{
    palarray = current_palarray;
    old_palarray = default_palarray;
}

void I_SetPalette(byte* palette)
{
    const byte *gammaptr = gammatable[usegamma];

    unsigned int i;
    
    for (i = 0; i < 256; i++)
    {
        int r = *palette++;
        int g = *palette++;
        int b = *palette++;

        r = gammaptr[r];
        g = gammaptr[g];
        b = gammaptr[b];

        uint16_t unpackedcol = ((r >> 3) << 11) | ((g >> 3) << 6) | ((b >> 3) << 1);
        uint32_t packedcol = (unpackedcol << 16) | unpackedcol;
        current_palarray[i] = packedcol;
    }
}

#include "z_zone.h"
#include "w_wad.h"
void I_SetDefaultPalette(void)
{
    I_SetPalette(W_CacheLumpName ("PLAYPAL",PU_CACHE));
    memcpy(default_palarray, current_palarray, sizeof(current_palarray));
}

void I_InitGraphics(void)
{
    I_SetDefaultPalette();

    palarray = current_palarray;

    frame_buffer = surface_alloc(FMT_RGBA16, SCREENWIDTH, SCREENHEIGHT);
    _dc = &frame_buffer;
    bufptr = _dc->buffer;

    // activate built-in font for RDPQ
    rdpq_text_register_font(FONT_BUILTIN_DEBUG_MONO, rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_MONO));
}
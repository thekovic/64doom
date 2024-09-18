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
// $Log:$
//
// DESCRIPTION:
//    Main program, simply calls D_DoomMain high level loop.
//
//-----------------------------------------------------------------------------
#include <limits.h>

#include <stdlib.h>

#include <inttypes.h>
#include <libdragon.h>

#include "doomdef.h"
#include "m_argv.h"
#include "d_main.h"
#include "g_game.h"

extern void DoomIsOver();

void check_and_init_mempak(void)
{
    joypad_accessory_type_t pak_type = joypad_get_accessory_type(JOYPAD_PORT_1);

    switch (pak_type)
    {
        case JOYPAD_ACCESSORY_TYPE_NONE:
        {
            printf("No accessory inserted!\n");
            break;
        }
        case JOYPAD_ACCESSORY_TYPE_CONTROLLER_PAK:
        {
            int err;
            // I left the below in the code #ifdef'd out for when I needed an easy way to nuke a controller pak
#if 0
            if (format_mempak(0))
            {
                printf("Error formatting mempak!\n");
            }
            else
            {
                printf("Memory card formatted!\n");
            }
#endif
            if ((err = validate_mempak(0)))
            {
                if (err == -3)
                {
                    printf("Mempak is not formatted! Formatting it automatically.\n");

                    if (format_mempak(0))
                    {
                        printf("Error formatting mempak!\n");
                    }
                    else
                    {
                        printf("Memory card formatted!\n");
                    }
                }
                else
                {
                    printf("Mempak bad or removed during read!\n");
                }
            }
            else
            {
                // this will print the details of each entry on the controller pak
#if 0
                for(int j = 0; j < 16; j++)
                {
                    entry_structure_t entry;

                    get_mempak_entry(0, j, &entry);

                    if(entry.valid)
                    {
                        printf("%s - %d blocks\n", entry.name, entry.blocks);
                    }
                    else
                    {
                        printf("(EMPTY)\n");
                    }
                }
#endif
                printf("\nFree space: %d blocks\n", get_mempak_free_space(0));
            }

            break;
        }
        default:
        {
            printf("Cannot read data from this accessory type!\n");
            break;
        }
    }
}

int main()
{
    console_init();
    console_set_render_mode(RENDER_AUTOMATIC);
    debug_init_isviewer();

    if (dfs_init(DFS_DEFAULT_LOCATION) != DFS_ESUCCESS)
    {
        printf("Could not initialize filesystem!\n");
        return 1;
    }

    joypad_init();
    rdpq_init();

    printf("LibdragonDOOM by the_kovic\n");
    printf("https://github.com/thekovic/LibdragonDOOM");
    printf("\nBased on 64Doom by jnmartin84\n");
    printf("ROM build: %s %s\n", __DATE__, __TIME__);

    if(!is_memory_expanded())
    {
        printf("***********************************\n");
        printf("Expansion Pak not found.\n");
        printf("It is required to run LibdragonDOOM.\n");
        printf("Please turn off the Nintendo 64,\ninsert Expansion Pak, and try again.\n");
        printf("***********************************\n");
        return 1;
    }

    printf("Expansion Pak found.\n");

    printf("Checking for Mempak:\n");
    check_and_init_mempak();

    D_DoomMain();
    
    DoomIsOver();

    return 0;
}

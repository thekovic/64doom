# LibdragonDOOM

LibdragonDOOM is a source port of classic PC DOOM 1/2 to the Nintendo 64 using the libdragon SDK.

## Current features:

- Adapted original software renderer
- Analog stick support
- MUS music playback
- All commercial IWADs playable
- Saving to controller pak (note: feature review needed)
- Pwad merging through Deutex

## Planned features:

- Hardware-accelerated RDP renderer
- Proper PWAD loading support
- Wider music support
- Saving to cartridge SRAM
- Many, many bug fixes

## Building

Setup libdragon `preview` branch based on instructions here: https://github.com/DragonMinded/libdragon/wiki/Installing-libdragon

Get a copy of an IWAD of a supported version of DOOM (DOOM 1 shareware version, DOOM 1 registered version, Ultimate DOOM, DOOM 2, Plutonia, TNT).

Set two variables in the Makefile:

- `IWAD_DIRECTORY` -- the path to the directory that contains your IWAD file

- `IWAD_PREFIX` -- the actual IWAD filename prefix (one of `doom1` (Shareware version), `doomr` (Registered version), `doom` (Ultimate DOOM), `doom2`, `plutonia`, `tnt` - these are case sensitive. Must be lowercase and your WAD filename must be lowercase i.e. `doom2.wad`)

And run `make`.

## PWAD support

Until this is reworked you can create merged iwads directly using the makefile. This depends on a fan favorite tool called Deutex

Build it from source located here  and place the root of the repo.

- `PWAD_DIRECTORY` -- the path to the directory that contains your PWAD file

- `PWAD_PREFIX` -- the actual PWAD filename prefix. Again same thing as the normal IWAD prefix.

You run `make pwad` then `make` as normal.

## Saving the game (note: feature review needed)

LibdragonDOOM uses the Controller Pak to save and load your game progress from the Save Game / Load Game menu options.

One savegame slot is presented, mapped to a single note on the Controller Pak.

The name of the note is the same as the game version (`$IWAD_PREFIX`) you are playing.

When you save or load a game, you will see a message on the DOOM HUD if it is successful:

`SAVED GAME TO MEMPAK`

`LOADED GAME FROM MEMPAK`

Savegames are compressed using the lzfx library. However, the compressed saves are still large relative to the capacity of the Controller Pak. 

In the event that there is not enough free space on the Controller Pak to save the game, you will see a message on the DOOM HUD:

`NOT ENOUGH SPACE FOR SAVE (NEED #, HAVE #)`

It is very likely you will see this message if you have notes from other games on your Controller Pak so it is desirable to play with a dedicated, initially empty Controller Pak. Despite that, it is possible that this may happen even with an empty Controller Pak if the map is too large, too many enemies are alive, too many items are uncollected, etc.

## Controls

- D-Pad Up / Analog Stick Up : Move forwards
- D-Pad Down / Analog Stick Down : Move backwards
- D-Pad Left / Analog Stick Left : Turn left
- D-Pad Right / Analog Stick Right : Turn right
- C-Left : Strafe left
- C-Right : Strafe right
- A Button : Switch to previous weapon
- B Button : Switch to next weapon
- C-Up : Toggle auto-map
- L Button : ENTER key (confirms selection in menu)
- R Button : Toggle always run on/off
- Z Button : Fire
- C-Down : Use (open doors, flip switches)
- Start : ESC key (brings menu up/down)

## Third-party credits

- Based on 64Doom by jnmartin84: https://github.com/jnmartin84/64doom
- Original DOOM source code by id Software: https://github.com/id-Software/DOOM
- Sample-based MUS playback and digital sound code by ChillyWilly
- Generic C hashtable by Dennis Francis: https://github.com/dennisfrancis/generic-hashtable
- LZFX codec: http://lzfx.googlecode.com (dead link)
- Screen wipe code from Chocolate Doom: https://github.com/chocolate-doom/chocolate-doom/blob/master/src/doom/f_wipe.c

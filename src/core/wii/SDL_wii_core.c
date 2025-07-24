/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2023 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include "../../SDL_internal.h"

#include "SDL_main.h"

#include "../../video/ogc/SDL_ogcevents_c.h"

#ifdef main
#undef main
#endif

/* Standard includes */
#include <stdio.h>

/* OGC includes */
#include <fat.h>
#include <ogc/usbmouse.h>
#include <ogcsys.h>
#include <wiikeyboard/keyboard.h>
#include <wiiuse/wpad.h>

/* moved this stuff out of SDL_main, since it shouldn't be there
 * (SDL should be able to run perfectly fine without a SDLmain.
 *  this is even clarified in the docs!) */

static void WII_ShutdownCB(void)
{
    OGC_PowerOffRequested = true;
}

static void WII_ResetCB(void)
{
    OGC_ResetRequested = true;
}

void WII_Init(void)
{
    /* make sure the contents of this function are only called once
     *
     * Ideally this stuff would be called only when it is actually
     * necessary (e.g. mouse & keyboard are unnecessary for users
     * who only need the audio portion of SDL) but this is OK for
     * now. */
    static int init = 0;
    if (init) return;
    init = 1;

    // Wii Power/Reset buttons
    WPAD_Init();
    WPAD_SetPowerButtonCallback((WPADShutdownCallback)WII_ShutdownCB);
    SYS_SetPowerCallback(WII_ShutdownCB);
    SYS_SetResetCallback((resetcallback)WII_ResetCB);
    // TODO OGC_InitVideoSystem();// do that in SDL_VideoInit, *not* here!!
    WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC_IR);
    WPAD_SetVRes(WPAD_CHAN_ALL, 640, 480);

    MOUSE_Init();
    KEYBOARD_Init(NULL);
    /* why is this here? where do we use libfat? */
    fatInitDefault();
}

/* vi: set ts=4 sw=4 expandtab: */

/// PPP - Surface and image header

#ifndef _CSURFACE_H
    #define _CSURFACE_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "CScreen.h"
#include "CLog.h"

class CSurface
{
public:

    static int              BG_W;
    static int              BG_H;
    static int              Sprites_W;
    static int              Sprites_H;

    CSurface();
    static SDL_Rect clips[600];

    static SDL_Texture*     OnLoad(const char* file);
    static void             OnDraw(SDL_Texture* tex, SDL_Rect box, SDL_Rect* clip);
    static void             Set_Clips();
    static bool             Init();

};


#endif // CSURFACE_H

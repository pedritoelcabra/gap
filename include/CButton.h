/// PPP

#ifndef _CBUTTON_H
#define _CBUTTON_H

#include <string>
#include <sstream>
#include <stdlib.h>
#include <memory>
#include <SDL2/SDL.h>
#include "SDL2/SDL_gpu.h"
#include <CGuiObject.h>
#include <CUnit.h>

typedef std::weak_ptr<CUnit> unit_weak_ptr;
class CGUIObject;

class CButton : public CGUIObject{
public:
    CButton (int x, int y, int clip, unit_weak_ptr object);
    CButton (int x, int y, int clip, int id, std::string caption);
    CButton(int x, int y, int clipnumber, int w, int h);

    bool                Render();
    unit_weak_ptr         GetObject();
    int                 GetAction();


    const int           TYPE_MENU = 1;
    const int           TYPE_CONTEXT = 2;
private:

    int                 action;
    std::string         name;
    unit_weak_ptr       object;
    int                 ParentMenu;
    int                 fontSize;
    SDL_Rect            captionRect;
    GPU_Rect            captionRectGL;
    static GPU_Rect     spriteClip(int clipnumber);
    std::string         caption;
    void                CenterCaption();
};

#endif // _CBUTTON_H

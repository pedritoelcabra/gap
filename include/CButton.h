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
#include "CBuilding.h"

typedef std::weak_ptr<CUnit> unit_weak_ptr;
typedef std::weak_ptr<CBuilding> build_weak_ptr;
class CGUIObject;

class CButton : public CGUIObject{
public:
    CButton (int x, int y, int clip, unit_weak_ptr object);
    CButton (int x, int y, int clip, build_weak_ptr building);
    CButton (int x, int y, int clip, int id, std::string caption);
    CButton(int x, int y, int clipnumber, int w, int h);
    CButton(int x, int y, int clipnumber, int w, int h, CRecipe& recipe);
    CButton(int x, int y, int clipnumber, int w, int h, build_weak_ptr building);


    bool                Render();
    unit_weak_ptr       GetUnit(){                                      return unitPtr;};
    build_weak_ptr      GetBuilding(){                                  return buildingPtr;};
    build_weak_ptr      GetWorkerBuilding(){                            return workerBuilding;};
    int                 GetAction(){                                    return action;};
    CRecipe*            GetRecipe(){                                    return recipe;};


    const int           TYPE_MENU = 1;
    const int           TYPE_CONTEXT = 2;
private:

    int                 action;
    int                 clipnumber;
    std::string         name;
    unit_weak_ptr       unitPtr;
    build_weak_ptr      buildingPtr;
    build_weak_ptr      workerBuilding;
    int                 ParentMenu;
    int                 fontSize;
    SDL_Rect            captionRect;
    GPU_Rect            captionRectGL;
    static GPU_Rect     spriteClip(int clipnumber);
    std::string         caption;
    void                CenterCaption();
    CRecipe*            recipe = nullptr;

    GPU_Image*          captionImage;
    void                Init(int clipnumber);
};

#endif // _CBUTTON_H

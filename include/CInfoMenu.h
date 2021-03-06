#ifndef CINFOMENU_H
#define CINFOMENU_H

#include "SDL2/SDL_gpu.h"
#include <CMenu.h>
#include "CAction.h"
#include "CUnit.h"


class CInfoMenu : public CMenu
{
    public:
        CInfoMenu(unit_weak_ptr ptr);
        CInfoMenu(build_weak_ptr ptr);
        CInfoMenu(tile_weak_ptr ptr);
        virtual ~CInfoMenu();

        void                    Render();
        void                    RenderLine(std::string text, int fontSize);
        void                    Clicked(CButton button);
        bool                    HandleLClickUp(int x, int y);
        bool                    HandleMouseMovement(int x, int y);
        bool                    OnMouseWheel(bool Up, int x, int y);

    protected:

    private:
        unit_weak_ptr           myUnit;
        build_weak_ptr          myBuilding;
        tile_weak_ptr           myTile;
        int                     x;
        int                     y;
        int                     h;
        int                     w;
        int                     yOff;
        int                     yScroll = 0;
        CRecipe*                selectedRecipe = nullptr;
        build_weak_ptr          selectecWorkerBuilding;
        GPU_Rect                imageBox;
};

#endif // CINFOMENU_H

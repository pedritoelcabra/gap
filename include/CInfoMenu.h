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
        virtual ~CInfoMenu();

        void                    Render();
        void                    RenderLine(std::string text, int fontSize);
        void                    Clicked(CButton button);

    protected:

    private:
        unit_weak_ptr           myUnit;
        build_weak_ptr          myBuilding;
        int                     x;
        int                     y;
        int                     h;
        int                     w;
        int                     yOff;
};

#endif // CINFOMENU_H

#ifndef CCONTEXTMENU_H
#define CCONTEXTMENU_H

#include <CMenu.h>
#include "CAction.h"
#include "CUnit.h"


class CContextMenu : public CMenu
{
    public:
        CContextMenu(int x, int y);
        virtual ~CContextMenu();
        void                    Clicked(CButton button);
    protected:
    private:
};

#endif // CCONTEXTMENU_H

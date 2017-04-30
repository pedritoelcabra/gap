#ifndef CBUILDINGMENU_H
#define CBUILDINGMENU_H

#include <CMenu.h>
#include "CBuildingType.h"


class CBuildingMenu : public CMenu
{
    public:
        CBuildingMenu();
        virtual ~CBuildingMenu();

        void            Clicked(CButton button);
    protected:

    private:
};

#endif // CBUILDINGMENU_H

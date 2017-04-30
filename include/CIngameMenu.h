#ifndef CINGAMEMENU_H
#define CINGAMEMENU_H

#include <CMenu.h>


class CIngameMenu : public CMenu
{
    public:
        CIngameMenu();
        virtual ~CIngameMenu();
        void                    Clicked(CButton button);

    protected:

    private:
};

#endif // CINGAMEMENU_H

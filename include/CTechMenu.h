#ifndef CTECHMENU_H
#define CTECHMENU_H

#include <CMenu.h>


class CTechMenu : public CMenu
{
    public:
        CTechMenu();
        virtual ~CTechMenu();

        void            Clicked(CButton button);
        void            LoadTechs();

    protected:

    private:
};

#endif // CTECHMENU_H

#include "CIngameMenu.h"
#include "CGame.h"


extern CGame GAP;

CIngameMenu::CIngameMenu()
{
    int offset = 0;
    CButton button = CButton(0, 0, 1, 1, "Menu" );
    PopUpButtons.push_back(button);

    offset += button.GetW();
    CButton button2 = CButton(offset, 0, 1, 2, "Buildings" );
    PopUpButtons.push_back(button2);

}

CIngameMenu::~CIngameMenu()
{
    //dtor
}

void CIngameMenu::Clicked(CButton button){
    GAP.MenuManager.HideContextMenus();

    switch(button.GetAction()){
        case 1:
            GAP.Exit();
            break;
        case 2:
            GAP.MenuManager.ShowBuildingMenu();
            break;
    }
}

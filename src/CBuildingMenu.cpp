#include "CBuildingMenu.h"
#include "CGame.h"

extern CGame GAP;



CBuildingMenu::CBuildingMenu()
{

    int tcount = 0;
    int xoff = 50;
    int yoff = 50;

    CButton button = CButton(xoff, yoff + (tcount * 30), 1, -1, "Demolish" );
    PopUpButtons.push_back(button);

    for(CBuildingType t : GAP.BuildingManager.GetBuildingTypes()){

        CButton button = CButton(xoff, yoff + (tcount * 30) + 30, 1, tcount, t.GetName() );
        PopUpButtons.push_back(button);
        tcount++;
    }

}

CBuildingMenu::~CBuildingMenu()
{
    //dtor
}

void CBuildingMenu::Clicked(CButton button){
    if(button.GetAction() == -1){
        GAP.MenuManager.MousePointerDemolition();
        return;
    }
    GAP.MenuManager.MousePointerBuilding(button.GetAction());
}

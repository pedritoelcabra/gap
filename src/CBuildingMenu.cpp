#include "CBuildingMenu.h"
#include "CGame.h"

extern CGame GAP;



CBuildingMenu::CBuildingMenu(){

    int tcount = -1;
    int xoff = 300;
    int yoff = 50;
    int initialY = yoff;
    int buttonHeight = 30;
    int buttonWidth = 150;

    CButton button = CButton(xoff, yoff, 1, -1, "Demolish" );
    yoff += buttonHeight;
    PopUpButtons.push_back(button);

    for(CBuildingType t : GAP.BuildingManager.GetBuildingTypes()){
        tcount++;
        if(!t.RequirementsMet()){
            continue;
        }
        if(yoff + buttonHeight > static_cast<int>(CScreen::screen_h)){
            yoff = initialY;
            xoff += buttonWidth;
        }
        CButton button = CButton(xoff, yoff, 1, tcount, *(t.GetName()) );
        yoff += buttonHeight;
        PopUpButtons.push_back(button);
    }

}

CBuildingMenu::~CBuildingMenu(){
    //dtor
}

void CBuildingMenu::Clicked(CButton button){
    if(button.GetAction() == -1){
        GAP.MenuManager.MousePointerDemolition();
        return;
    }
    GAP.MenuManager.MousePointerBuilding(button.GetAction());
}

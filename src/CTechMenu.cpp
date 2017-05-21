#include "CTechMenu.h"
#include "CGame.h"

extern CGame GAP;

CTechMenu::CTechMenu(){
    LoadTechs();
}

CTechMenu::~CTechMenu(){
    //dtor
}

void CTechMenu::Clicked(CButton button){
    GAP.TechManager.StartResearch(button.GetAction());
    GAP.MenuManager.HideTechMenus();
}

void CTechMenu::LoadTechs(){

    PopUpButtons.clear();

    int tcount = 0;
    int xoff = 300;
    int yoff = 50;
    int initialY = yoff;
    int buttonHeight = 30;
    int buttonWidth = 150;

    for(auto t : *(GAP.TechManager.GetTechs())){
        if(!t.second.IsAvailable()){
            continue;
        }
        if(t.second.IsResearched()){
            continue;
        }
        if(yoff + buttonHeight > static_cast<int>(CScreen::screen_h)){
            yoff = initialY;
            xoff += buttonWidth;
        }
        CButton button = CButton(xoff, yoff, 1, t.second.GetId(), *(t.second.GetName()) );
        yoff += buttonHeight;
        PopUpButtons.push_back(button);
        tcount++;
    }
}

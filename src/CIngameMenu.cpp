#include "CIngameMenu.h"
#include "CGame.h"
#include "CScreen.h"


extern CGame GAP;

CIngameMenu::CIngameMenu()
{
    int offset = 0;
    CButton button = CButton(0, 0, 1, 1, "Menu" );
    PopUpButtons.push_back(button);

    offset += button.GetW();
    CButton button4 = CButton(offset, 0, 4, 4, "P", 30 , 30);
    PopUpButtons.push_back(button4);

    offset += button4.GetW();
    CButton button2 = CButton(offset, 0, 1, 2, "Buildings" );
    PopUpButtons.push_back(button2);

    offset += button2.GetW();
    CButton button3 = CButton(offset, 0, 1, 3, *GAP.TechManager.CurrentTechName() );
    PopUpButtons.push_back(button3);

}

CIngameMenu::~CIngameMenu()
{
    //dtor
}

void CIngameMenu::Render()
{
    CMenu::Render();
    int lineX, lineY, lineMaxW, lineCurrentW;
    lineX = 333;
    lineY = 25;
    lineMaxW = 149;
    lineCurrentW = (lineMaxW * GAP.TechManager.CurrentTech()->GetProgress())
                    / GAP.TechManager.CurrentTech()->GetCost();
    GPU_Line(GAP.MainGLWindow, lineX, lineY, lineX + lineMaxW, lineY, {0,0,0,255} );
    GPU_Line(GAP.MainGLWindow, lineX, lineY, lineX + lineCurrentW, lineY, {100,100,255,255} );
}

void CIngameMenu::Clicked(CButton button){
    GAP.MenuManager.HideContextMenus();

    switch(button.GetAction()){
        case 1:
            // GAP.Exit();
            break;
        case 2:
            GAP.MenuManager.ClearMenus();
            GAP.MenuManager.ShowBuildingMenu();
            break;
        case 3:
            GAP.MenuManager.ClearMenus();
            GAP.MenuManager.ShowTechMenu();
            break;
        case 4:
            GAP.MenuManager.MousePointerBuilding(GAP.BuildingManager.GetRoadType());
            break;
    }
}

#include "CContextMenu.h"
#include "CGame.h"


extern CGame GAP;

CContextMenu::CContextMenu(int x, int y){

    int offset = y;

    std::vector<unit_weak_ptr> npcs = GAP.UnitsAtTile(GAP.GetMouseTileX(),GAP.GetMouseTileY());

    for(auto e : npcs){
        if(auto s = e.lock()){
            CButton button = CButton(x, offset, 1, unit_weak_ptr(s) );
            offset += button.GetH();
            PopUpButtons.push_back(button);
        }
    }
}

CContextMenu::~CContextMenu(){
    //dtor
}

void CContextMenu::Clicked(CButton button){

    GAP.MenuManager.HideContextMenus();

    unit_weak_ptr unit_ptr = button.GetUnit();
    if(auto s = unit_ptr.lock()){
        GAP.MenuManager.ShowInfoUnit(unit_ptr);
        return;
    }

}

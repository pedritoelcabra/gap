#include "CContextMenu.h"
#include "CGame.h"


extern CGame GAP;

CContextMenu::CContextMenu(int x, int y){

    int offset = y;

    std::vector<unit_weak_ptr> npcs = GAP.UnitsAtTile(GAP.getMouseTileX(),GAP.getMouseTileY());

    for(auto e : npcs)
    {
        if(auto s = e.lock()){
            CButton button = CButton(x, offset, 1, unit_weak_ptr(s) );
            offset += button.getH();
            PopUpButtons.push_back(button);
        }
    }
}

CContextMenu::~CContextMenu(){
    //dtor
}

void CContextMenu::Clicked(CButton button){
    //
    unit_weak_ptr unit_ptr = button.GetObject();
    if(auto s = unit_ptr.lock()){
        GAP.MenuManager.ShowInfoUnit(unit_ptr);
//        if(s->getAssignment() == CAction::followUnit){
//            s->SetIdleAssignment();
//        }else{
//            s->SetFollowAssignment(unit_weak_ptr(GAP.Player));
//        }
    }

    GAP.MenuManager.HideContextMenus();
}

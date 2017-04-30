/// PPP - Menu code

#include "CMenu.h"
#include "CGame.h"


extern CGame GAP;


CMenu::CMenu(){

}

CMenu::~CMenu(){

}

void CMenu::Render(){

    for(CButton& e : PopUpButtons)
    {
       e.Render();
    }
}

int CMenu::ButtonCount() const{
    return PopUpButtons.size();
}

bool CMenu::FindCollision(int x, int y){

    for(CButton& e : PopUpButtons)
    {
        if(e.FindCollision(x, y)){
            Clicked(e);
            return true;
        }
    }
    return false;
}

bool CMenu::HandleLClick(int x, int y){

    return FindCollision(x, y);
}

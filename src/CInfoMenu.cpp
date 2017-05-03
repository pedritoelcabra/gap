#include "CInfoMenu.h"
#include "CGame.h"


extern CGame GAP;

CInfoMenu::CInfoMenu(unit_weak_ptr ptr){
    myUnit = ptr;
    myBuilding.reset();
    w = CScreen::screen_w / 5;
    x = CScreen::screen_w - w;
    y = 0;
    h = CScreen::screen_h;
}

CInfoMenu::CInfoMenu(build_weak_ptr ptr){
    myBuilding = ptr;
    myUnit.reset();
    w = CScreen::screen_w / 5;
    x = CScreen::screen_w - w;
    y = 0;
    h = CScreen::screen_h;
}

CInfoMenu::~CInfoMenu(){
    //dtor
}

void CInfoMenu::Render(){
    PopUpButtons.clear();
    SDL_Color grey = {100, 100, 100, 255};
    SDL_Color red = {255, 100, 100, 255};
    SDL_Color green = {100, 255, 100, 255};
    GPU_RectangleFilled(GAP.MainGLWindow,x,y,x + w,y + h,grey);
    yOff = 10;

    if(auto s = myUnit.lock()){
        std::stringstream tmp;
        RenderLine("Unit Overview:", 16);
        tmp << "TileX: " << s->GetTileX() << " TileY: " << s->GetTileY();
        RenderLine(tmp.str().c_str(), 16);
        tmp.str(std::string());
        RenderLine(s->GetAssignmentName(), 16);
        RenderLine(s->GetThought(), 16);
        if(s->GetCarriedItem()){
            tmp << "Carrying 1 " << CGood::GetResourceName(s->GetCarriedItem());
            RenderLine(tmp.str().c_str(), 16);
            tmp.str(std::string());
        }
        RenderLine("Home:", 16);
        if(auto homeb = s->GetHome().lock()){
            CButton button = CButton(x + 10, yOff, 1, s->GetHome() );
            yOff += button.GetH();
            PopUpButtons.push_back(button);
            GAP.TextureManager.DrawConnectionLine(s->GetTileX(),s->GetTileY(),homeb->GetDoor().first,homeb->GetDoor().second, green);
        }
        RenderLine("Workplace:", 16);
        if(auto homeb = s->GetWorkplace().lock()){
            CButton button = CButton(x + 10, yOff, 1, s->GetWorkplace() );
            yOff += button.GetH();
            PopUpButtons.push_back(button);
            GAP.TextureManager.DrawConnectionLine(s->GetTileX(),s->GetTileY(),homeb->GetDoor().first,homeb->GetDoor().second, red);
        }
    }

    if(auto s = myBuilding.lock()){
        std::stringstream tmp;
        RenderLine("Building Overview:", 16);
        RenderLine(s->GetName(), 16);
        RenderLine(s->GetDescription(), 16);
        tmp << "DoorX: " << s->GetDoor().first << " DoorY: " << s->GetDoor().second;
        RenderLine(tmp.str().c_str(), 16);
        tmp.str(std::string());
        RenderLine("Inventory:", 16);
        for(auto const& x : s->GetInventory() ){
            if(x.second > 0){
                tmp << CGood::GetResourceName(x.first) << ": " << x.second;
                RenderLine(tmp.str().c_str(), 16);
                tmp.str(std::string());
            }
        }
        for(auto wb : s->GetConnections()){
            if(auto sb = wb.lock()){
                GAP.TextureManager.DrawConnectionLine(sb->GetDoor().first ,sb->GetDoor().second ,s->GetDoor().first,s->GetDoor().second, grey);
            }
        }
        RenderLine("Inhabitants:", 16);
        for(auto wu : s->GetInhabitants()){
            if(auto su = wu.lock()){
                CButton button = CButton(x + 10, yOff, 1, wu );
                yOff += button.GetH();
                PopUpButtons.push_back(button);
                GAP.TextureManager.DrawConnectionLine(su->GetTileX(),su->GetTileY(),s->GetDoor().first,s->GetDoor().second, green);
            }
        }
        RenderLine("Workers:", 16);
        for(auto wu : s->GetWorkers()){
            if(auto su = wu.lock()){
                CButton button = CButton(x + 10, yOff, 1, wu );
                yOff += button.GetH();
                PopUpButtons.push_back(button);
                GAP.TextureManager.DrawConnectionLine(su->GetTileX(),su->GetTileY(),s->GetDoor().first,s->GetDoor().second, red);
            }
        }
    }

    CMenu::Render();
}

void CInfoMenu::RenderLine(std::string text, int fontSize){

    SDL_Color black = {0, 0, 0};
    SDL_Surface* textSurface = TTF_RenderText_Solid( GAP.TextureManager.GetFont(20), text.c_str(), black );
    GPU_Image* texture = GPU_CopyImageFromSurface( textSurface );
    GPU_Rect Message_rect; //create a rect
    Message_rect.x = x + 10;  //controls the rect's x coordinate
    Message_rect.y = y + yOff; // controls the rect's y coordinte
    Message_rect.w = strlen(text.c_str())*10; // controls the width of the rect
    Message_rect.h = 20; // controls the height of the rect

    GAP.TextureManager.DrawTextureGL(texture, 0, &Message_rect, true);
    GPU_FreeImage(texture);
    SDL_FreeSurface( textSurface );
    yOff += 20;
}

void CInfoMenu::Clicked(CButton button){

    GAP.MenuManager.HideContextMenus();

    unit_weak_ptr unit_ptr = button.GetUnit();
    if(auto s = unit_ptr.lock()){
        GAP.MenuManager.ShowInfoUnit(unit_ptr);
        return;
    }

    build_weak_ptr build_ptr = button.GetBuilding();
    if(auto s = build_ptr.lock()){
        GAP.MenuManager.ShowInfoBuilding(build_ptr);
        return;
    }

}

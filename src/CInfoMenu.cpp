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
    SDL_Color grey = {100, 100, 100, 255};
    GPU_RectangleFilled(GAP.MainGLWindow,x,y,x + w,y + h,grey);
    yOff = 10;

    if(auto s = myUnit.lock()){
        std::stringstream tmp;
        RenderLine("Unit Overview:", 24);
        tmp << "TileX: " << s->getTileX() << " TileY: " << s->getTileY();
        RenderLine(tmp.str().c_str(), 24);
        tmp.str(std::string());
    }

    if(auto s = myBuilding.lock()){
        std::stringstream tmp;
        RenderLine("Building Overview:", 24);
        RenderLine(s->GetName(), 24);
        RenderLine(s->GetDescription(), 24);
        tmp << "DoorX: " << s->getDoor().first << " DoorY: " << s->getDoor().second;
        RenderLine(tmp.str().c_str(), 24);
        tmp.str(std::string());
    }
}

void CInfoMenu::RenderLine(std::string text, int fontSize){

    SDL_Color black = {0, 0, 0};
    SDL_Surface* textSurface = TTF_RenderText_Solid( GAP.TextureManager.GetFont(fontSize), text.c_str(), black );
    GPU_Image* texture = GPU_CopyImageFromSurface( textSurface );
    GPU_Rect Message_rect; //create a rect
    Message_rect.x = x + 10;  //controls the rect's x coordinate
    Message_rect.y = y + yOff; // controls the rect's y coordinte
    Message_rect.w = strlen(text.c_str())*8; // controls the width of the rect
    Message_rect.h = 15; // controls the height of the rect

    GAP.TextureManager.DrawTextureGL(texture, 0, &Message_rect, true);
    GPU_FreeImage(texture);
    SDL_FreeSurface( textSurface );
    yOff += 15;
}

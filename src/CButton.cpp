/// PPP

#include "CButton.h"
#include "CGame.h"

extern CGame GAP;

CButton::CButton(int x, int y, int clipnumber, unit_weak_ptr unit) : CGUIObject(x, y){
    box.h = 30;
    box.w = 151;
    fontSize = int(box.h / 4)*2;
    if(auto obj = unit.lock()){
        caption.append(*(obj->GetName()));
        unitPtr = unit;
    }
    Init(clipnumber);
}

CButton::CButton(int x, int y, int clipnumber, build_weak_ptr building) : CGUIObject(x, y){
    box.h = 30;
    box.w = 151;
    fontSize = int(box.h / 4)*2;
    if(auto obj = building.lock()){
        caption.append(*(obj->GetName()));
        buildingPtr = building;
    }
    Init(clipnumber);
}

CButton::CButton(int x, int y, int clipnumber, int id, std::string caption) : CGUIObject(x, y){
    box.h = 30;
    box.w = 151;
    fontSize = int(box.h / 4)*2;
    this->caption = caption;
    this->action = id;
    Init(clipnumber);
}

CButton::CButton(int x, int y, int clipnumber, int w, int h) : CGUIObject(x, y){
    box.h = h;
    box.w = w;
    fontSize = int(box.h / 4)*2;
    this->caption = "";
    Init(clipnumber);
}

CButton::CButton(int x, int y, int clipnumber, int w, int h, CRecipe& recipe_) : CGUIObject(x, y){
    box.h = h;
    box.w = w;
    fontSize = 10;
    std::stringstream tmp;
    tmp << recipe_.GetProductionPrio();
    this->caption = tmp.str();
    recipe = &recipe_;
    Init(clipnumber);
}

bool CButton::Render(){
    GAP.TextureManager.DrawTextureGL(&name, &clip, &box, true);

    if(caption.empty()){
        return 1;
    }

    GAP.TextureManager.DrawTextureGL(captionImage,0, &captionRectGL, true);
    return 1;
}

void CButton::Init(int clipnumber){
    name = "menusprites";
    box.x = x;
    box.y = y;

    switch(clipnumber){
        case 2:
            clip.x = 0;
            clip.y = 216;
            clip.w = 151;
            clip.h = 30;
            break;
        case 1:
            clip.x = 0;
            clip.y = 308;
            clip.w = 151;
            clip.h = 30;
            break;
        case 3:
            clip.x = 1;
            clip.y = 309;
            clip.w = 149;
            clip.h = 28;
            break;
    }

    const char* captionCHAR = caption.c_str();
    TTF_SizeText(GAP.TextureManager.GetFont(fontSize), captionCHAR, &captionRect.w, &captionRect.h);
    captionRectGL.x = ((box.w - captionRect.w) / 2) + box.x;
    captionRectGL.y = ((box.h - captionRect.h) / 2) + box.y;
    captionRectGL.h = captionRect.h;
    captionRectGL.w = captionRect.w;

    SDL_Color black = {0, 0, 0};
    SDL_Surface* textSurface = TTF_RenderText_Solid( GAP.TextureManager.GetFont(fontSize), caption.c_str(), black );
    captionImage = GPU_CopyImageFromSurface( textSurface );

    SDL_FreeSurface(textSurface);
}

void CButton::CenterCaption(){
    return;
}


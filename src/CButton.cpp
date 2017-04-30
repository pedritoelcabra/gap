/// PPP

#include "CButton.h"
#include "CGame.h"

extern CGame GAP;

CButton::CButton(int x, int y, int clipnumber, unit_weak_ptr object_) : CGUIObject(x, y){
    box.h = 30;
    box.w = 151;
    fontSize = int(box.h / 4)*2;
    clip = spriteClip(clipnumber);
    name = "menusprites";
    if(auto obj = object_.lock()){
        caption = "View ";
        // caption = (obj->getAssignment() != CAction::followUnit ? "Recruit " : "Leave ");
        caption.append(obj->getName());
        this->object = unit_weak_ptr(obj);
    }
    box.x = x;
    box.y = y;
}

CButton::CButton(int x, int y, int clipnumber, int id, std::string caption) : CGUIObject(x, y){
    box.h = 30;
    box.w = 151;
    fontSize = int(box.h / 4)*2;
    clip = spriteClip(clipnumber);
    name = "menusprites";
    this->caption = caption;
    this->action = id;
    box.x = x;
    box.y = y;
}

CButton::CButton(int x, int y, int clipnumber, int w, int h) : CGUIObject(x, y){
    box.h = h;
    box.w = w;
    box.x = x;
    box.y = y;
    fontSize = int(box.h / 4)*2;
    clip = spriteClip(clipnumber);
    name = "menusprites";
    this->caption = "";
}

bool CButton::Render(){
    GAP.TextureManager.DrawTextureGL(name, &clip, &box, true);

    if(caption.empty()){
        return 1;
    }
    CenterCaption();
    SDL_Color black = {0, 0, 0};
    SDL_Surface* textSurface = TTF_RenderText_Solid( GAP.TextureManager.GetFont(fontSize), caption.c_str(), black );
    GPU_Image* texture = GPU_CopyImageFromSurface( textSurface );

    GAP.TextureManager.DrawTextureGL(texture,0, &captionRectGL, true);
    GPU_FreeImage( texture );
    SDL_FreeSurface(textSurface);
    return 1;
}

GPU_Rect CButton::spriteClip(int clipnumber){
    GPU_Rect spriteClip = GPU_Rect();
    switch(clipnumber){
        case 2:
            spriteClip.x = 0;
            spriteClip.y = 216;
            spriteClip.w = 151;
            spriteClip.h = 30;
            break;
        case 1:
            spriteClip.x = 0;
            spriteClip.y = 308;
            spriteClip.w = 151;
            spriteClip.h = 30;
            break;
        case 3:
            spriteClip.x = 1;
            spriteClip.y = 309;
            spriteClip.w = 149;
            spriteClip.h = 28;
            break;
    }

    return spriteClip;
}

void CButton::CenterCaption(){
    const char* captionCHAR = caption.c_str();
    TTF_SizeText(GAP.TextureManager.GetFont(fontSize), captionCHAR, &captionRect.w, &captionRect.h);
    captionRectGL.x = ((box.w - captionRect.w) / 2) + box.x;
    captionRectGL.y = ((box.h - captionRect.h) / 2) + box.y;
    captionRectGL.h = captionRect.h;
    captionRectGL.w = captionRect.w;
    return;
}

unit_weak_ptr CButton::GetObject(){
    return object;
}

int CButton::GetAction(){
    return action;
}

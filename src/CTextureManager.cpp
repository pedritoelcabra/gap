#include "CTextureManager.h"
#include "CGame.h"

extern CGame GAP;

CTextureManager::CTextureManager()
{
    textureCount = 0;
}

CTextureManager::~CTextureManager()
{
    //dtor
}

void CTextureManager::Init(){

    int v = 0;
    for(int i = 0; i<21; i++){
        for(int k = 0; k < 13; k++){
            SpriteClips[ v ].x = k*64;
            SpriteClips[ v ].y = i*64;
            SpriteClips[ v ].w = 64;
            SpriteClips[ v ].h = 64;
            v++;
        }
    }
}

bool CTextureManager::LoadTexture(std::string name, std::string path){
    Textures[name] = CSurface::OnLoad(path.c_str());
    CLog::Write(name);
    if (Textures[name]  == NULL){
        CLog::Write("Could not load texture: ");
        CLog::Write(name);
        CLog::Write("\n");
        return false;
    }

    return true;
}

int CTextureManager::LoadTextureGL(std::string name, std::string path){
    TexturesGL[name] = GPU_LoadImage(path.c_str());
    CLog::Write(name);
    if (TexturesGL[name]  == NULL){
        CLog::Write("Could not load texture: ");
        CLog::Write(name);
        CLog::Write("\n");
        return false;
    }

    return true;

}

void CTextureManager::DrawTextureGL(std::string name, GPU_Rect* src_rect, GPU_Rect* dest_rect, bool absolutePos){
    DrawTextureGL(TexturesGL.at(name), src_rect, dest_rect, absolutePos);
}

void CTextureManager::DrawTextureGL(GPU_Image* texture, GPU_Rect* src_rect, GPU_Rect* dest_rect, bool absolutePos){
    if(absolutePos){
        DrawRect.x = dest_rect->x;
        DrawRect.y = dest_rect->y;
    }else{
        DrawRect.x = dest_rect->x - GAP.MainViewport.x;
        DrawRect.y = dest_rect->y - GAP.MainViewport.y;
    }
    DrawRect.h = dest_rect->h;
    DrawRect.w = dest_rect->w;
    if(!absolutePos){
        DrawRect.x = DrawRect.x * GAP.ZoomLvl();
        DrawRect.y = DrawRect.y * GAP.ZoomLvl();
        DrawRect.h = DrawRect.h * GAP.ZoomLvl();
        DrawRect.w = DrawRect.w * GAP.ZoomLvl();
    }
    GPU_BlitRect(texture, src_rect, GAP.MainGLWindow, &DrawRect);
}

bool CTextureManager::LoadFont(std::string path){
    for(int i = 2; i<= 40; i+=2){
        Fonts[i] = TTF_OpenFont(path.c_str(), i);
    }

    return true;
}

GPU_Image* CTextureManager::GetTexture(std::string name){

    return TexturesGL.at(name);
}

void CTextureManager::SetTexture(SDL_Texture* tex, std::string name){

    Textures.at(name) = tex;
}

TTF_Font* CTextureManager::GetFont(int fontSize){
    return Fonts.at(fontSize);
}

GPU_Rect* CTextureManager::GetClip(int frame){
    if(frame > 0 && frame < (spriteCount-1)){
        return &SpriteClips[frame];
    }
    std::terminate();
}

void CTextureManager::DrawHighLightCircle(int tileX, int tileY, int tileRadius, SDL_Color  color){
    GPU_CircleFilled	(	GAP.MainGLWindow,
        ((tileX * CScreen::tileWidth) - GAP.MainViewport.x)* GAP.ZoomLvl(),
        ((tileY * CScreen::tileWidth) - GAP.MainViewport.y)* GAP.ZoomLvl(),
        ((tileRadius * CScreen::tileWidth))* GAP.ZoomLvl(),
        color
        )	;
}

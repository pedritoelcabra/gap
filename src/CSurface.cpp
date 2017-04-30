/// PPP - Surface and image code

#include "CSurface.h"
#include "CGame.h"

extern CGame GAP;
SDL_Rect    CSurface::clips[600];
int         CSurface::BG_W;
int         CSurface::BG_H;
int         CSurface::Sprites_W;
int         CSurface::Sprites_H;

CSurface::CSurface(){

}

SDL_Texture* CSurface::OnLoad(const char* file){

    SDL_Texture* loaded_image = NULL;
    SDL_Surface* loaded_surf = IMG_Load(file);
    if ( loaded_surf != NULL){
        loaded_image = SDL_CreateTextureFromSurface(GAP.getRenderer(), loaded_surf);
		SDL_FreeSurface( loaded_surf );
    }else{
        CLog::Write(IMG_GetError());
    }
    return loaded_image;
}

void CSurface::OnDraw(SDL_Texture* tex, SDL_Rect box, SDL_Rect* clip = NULL){
    SDL_Rect src;
    if (clip != NULL){
        src.x = clip->x;
        src.y = clip->y;
        src.w = clip->w;
        src.h = clip->h;
    }else{
        src.x = 0;
        src.y = 0;
        SDL_QueryTexture(tex, NULL, NULL, &src.w, &src.h);
    }
    SDL_RenderCopy(GAP.getRenderer(), tex, &src, &box);
}


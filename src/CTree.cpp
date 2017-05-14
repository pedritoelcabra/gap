#include "CTree.h"
#include "CGame.h"

extern CGame GAP;

CTree::CTree()
{
    clip.w = 128;
    clip.h = 128;
}

CTree::~CTree()
{
    //dtor
}

void CTree::Set(int x_, int y_, int type_, int variety_){
    switch(variety_){
    case 0:
        clip.x = 0;
        clip.y = 0;
        break;
    case 1:
        clip.x = 128;
        clip.y = 0;
        break;
    case 2:
        clip.x = 0;
        clip.y = 128;
        break;
    case 3:
        clip.x = 128;
        clip.y = 128;
        break;
    }
    switch(type_){
    case 1:
        textureName = "tree";
        box.w = 96;
        box.h = 96;
        box.x = x_ - CScreen::tileWidth;
        box.y = y_ - (CScreen::tileWidth * 2);
        break;
    case 2:
        textureName = "rock";
        box.w = 32;
        box.h = 32;
        box.x = x_;
        box.y = y_;
        break;
    }
}

bool CTree::Render(){
    GAP.TextureManager.DrawTextureGL(&textureName, &clip, &box);
    return 1;
}

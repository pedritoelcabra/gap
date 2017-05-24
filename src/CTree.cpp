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
    box.w = CScreen::tileWidth;
    box.h = CScreen::tileWidth;
    box.x = x_;
    box.y = y_;
    switch(type_){
        case CGood::wood:
            textureName = "tree";
            box.w = 96;
            box.h = 96;
            box.x = x_ - CScreen::tileWidth;
            box.y = y_ - (CScreen::tileWidth * 2);
            break;
        case CGood::stone:
            textureName = "rock";
        case CGood::coal:
            textureName = "coal";
        case CGood::tinOre:
            textureName = "tin";
        case CGood::copperOre:
            textureName = "copper";
        case CGood::ironOre:
            textureName = "iron";
        case CGood::silverOre:
            textureName = "silver";
        case CGood::goldOre:
            textureName = "gold";
        case CGood::lapis:
            textureName = "lapis";
        case CGood::gemstones:
            textureName = "gems";
        case CGood::ebony:
            textureName = "ebony";
            box.w = 96;
            box.h = 96;
            box.x = x_ - CScreen::tileWidth;
            box.y = y_ - (CScreen::tileWidth * 2);
        case CGood::lime:
            textureName = "lime";
        case CGood::marble:
            textureName = "marble";
            break;
    }
}

bool CTree::Render(){
    GAP.TextureManager.DrawTextureGL(&textureName, &clip, &box);
    return 1;
}

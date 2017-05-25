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
            break;
        case CGood::coal:
            textureName = "coal";
            break;
        case CGood::tinOre:
            textureName = "tin";
            break;
        case CGood::copperOre:
            textureName = "copper";
            break;
        case CGood::ironOre:
            textureName = "iron";
            break;
        case CGood::silverOre:
            textureName = "silver";
            break;
        case CGood::goldOre:
            textureName = "gold";
            break;
        case CGood::lapis:
            textureName = "lapis";
            break;
        case CGood::gemstones:
            textureName = "gems";
            break;
        case CGood::ebony:
            textureName = "ebony";
            box.w = 96;
            box.h = 96;
            box.x = x_ - CScreen::tileWidth;
            box.y = y_ - (CScreen::tileWidth * 2);
            break;
        case CGood::lime:
            textureName = "lime";
            break;
        case CGood::marble:
            textureName = "marble";
            break;
    }
}

bool CTree::Render(bool absolute){
    GAP.TextureManager.DrawTextureGL(&textureName, &clip, &box, absolute);
    return 1;
}

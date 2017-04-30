#include "CTree.h"
#include "CGame.h"

extern CGame GAP;

CTree::CTree()
{
    box.w = 96;
    box.h = 96;
    clip.x = 0;
    clip.y = 0;
    clip.h = 128;
    clip.w = 128;
    textureName = "tree1";
}

CTree::~CTree()
{
    //dtor
}

void CTree::Set(int x_, int y_, int type_){
    box.x = x_ - CScreen::tileWidth;
    box.y = y_ - (CScreen::tileWidth * 2);
    type = type_;
}

bool CTree::Render(){
    GAP.TextureManager.DrawTextureGL(textureName, &clip, &box);
    return 1;
}

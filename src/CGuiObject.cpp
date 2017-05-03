/// GAP

#include "CGuiObject.h"
#include "CGame.h"

extern CGame GAP;

CGUIObject::CGUIObject(int x, int y){
    this->x = x;
    this->y = y;
    box.x = this->x;
    box.y = this->y;
    z = rand() % 1000;
}

bool CGUIObject::FindCollision(int x, int y){
    if(x < box.x){
        return false;
    }
    if(y < box.y){
        return false;
    }
    if(x > box.x + box.w){
        return false;
    }
    if(y > box.y + box.h){
        return false;
    }
    return true;
}

bool CGUIObject::FindTileCollision(int x, int y){
    if(x == tileX && y == tileY){
        return true;
    }
    return false;
}

int CGUIObject::GetTileFlightSquareDistance(int x_, int y_) const{
    return std::max(std::abs(tileX-x_), std::abs(tileY-y_));
}

int CGUIObject::GetTileFlightRoundDistance(int x_, int y_) const{
    int xdist = std::abs(tileX-x_);
    int ydist = std::abs(tileY-y_);
    return sqrt((xdist*xdist)+(ydist*ydist));
}

int CGUIObject::GetTileDirection(int x_, int y_) const{
    if(x_ == tileX){
        return y_ < tileY ? 0 : 4;
    }
    if(tileY == y_){
        return x_ < tileX ? 2 : 6;
    }
    if(x_ < tileX){
        return y_ < tileY ? 1 : 3;
    }
    if(x_ > tileX){
        return y_ < tileY ? 7 : 5;
    }
    return 4;
}

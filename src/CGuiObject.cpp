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

int CGUIObject::getX() const{
    return this->x;
}

int CGUIObject::getZ() const{
    return this->z;
}

int CGUIObject::getTileX() const{
    return tileX;
}

int CGUIObject::getY() const{
    return this->y;
}

int CGUIObject::getTileY() const{
    return tileY;
}

int CGUIObject::setX(int x){
    this->x = x;
    return this->x;
}

int CGUIObject::setY(int y){
    this->y = y;
    return this->y;
}

int CGUIObject::getCenterX() const{
    return x + (box.w/2);
}

int CGUIObject::getCenterY() const{
    return y + (box.h/2);
}

int CGUIObject::getW() const{
    return box.w;
}

int CGUIObject::getH() const{
    return box.h;
}

int CGUIObject::GetId(){
    return id;
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

int CGUIObject::getTileFlightSquareDistance(int x_, int y_) const{
    return std::max(std::abs(tileX-x_), std::abs(tileY-y_));
}

int CGUIObject::getTileFlightRoundDistance(int x_, int y_) const{
    int xdist = std::abs(tileX-x_);
    int ydist = std::abs(tileY-y_);
    return sqrt((xdist*xdist)+(ydist*ydist));
}

int CGUIObject::getTileDirection(int x_, int y_) const{
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

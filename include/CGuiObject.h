/// PPP

#ifndef _CGUIOBJECT_H
#define _CGUIOBJECT_H

#include <string>
#include <sstream>
#include <stdlib.h>
#include <algorithm>
#include <memory>
#include <SDL2/SDL.h>
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_gpu.h"
#include "CScreen.h"
#include "CAction.h"
#include "CPathfinder.h"


class CGUIObject{


public:
    CGUIObject (){};
    CGUIObject (int x, int y);
    ~CGUIObject(){};


        int                 GetX() const {                                      return x;};
        int                 GetTileX() const{                                   return tileX;};
        int                 GetY() const {                                      return y;};
        int                 GetTileY() const{                                   return tileY;};
        int                 GetW() const{                                       return box.w;};
        int                 GetH() const{                                       return box.h;};
        int                 GetId(){                                            return id;};
        int                 GetTileFlightSquareDistance(int x, int y) const;
        int                 GetTileFlightRoundDistance(int x, int y) const;
        int                 GetTileDirection(int x, int y) const;

        int                 SetX(int x_){                                        x = x_; return x;};
        int                 SetY(int y_){                                        y = y_; return y;};

        bool                FindCollision(int x, int y);
        bool                FindTileCollision(int x, int y);

        virtual std::string GetName() const { std::terminate(); };
        virtual void        SetFacing(int direction) { std::terminate(); };
        virtual void        SetAnimation(std::string) { std::terminate(); };
        virtual bool        Render() { std::terminate(); };
        virtual bool        RenderOnMinimap(int x, int y) { std::terminate(); };
        virtual SDL_Color   GetMinimapColor() { std::terminate(); };
        virtual void        Update() { std::terminate(); };
        virtual float       GetMinCollision() { std::terminate(); };
        virtual float       GetMaxCollision() { std::terminate(); };
        virtual int         GetAssignment() { std::terminate(); };
        virtual int         GetResource() { return false; };
protected:

        int                 id;
        int                 x;
        int                 tileX;
        int                 y;
        int                 tileY;
        int                 z;
        int                 action;
        std::string         textureName;
        GPU_Rect            box;       //unit surface
        GPU_Rect            clip;


};

#endif // _CGUIOBJECT_H


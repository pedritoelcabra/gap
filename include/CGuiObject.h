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
#include "CSurface.h"
#include "CAction.h"
#include "CPathfinder.h"


class CGUIObject{


public:
    CGUIObject (){};
    CGUIObject (int x, int y);
    ~CGUIObject(){};


        int                 getX() const;
        int                 getZ() const;
        int                 setX(int x);
        int                 getCenterX() const;
        int                 getTileX() const;
        int                 getY() const;
        int                 setY(int y);
        int                 getCenterY() const;
        int                 getTileY() const;
        int                 getW() const;
        int                 getH() const;
        int                 GetId();
        int                 getTileFlightSquareDistance(int x, int y) const;
        int                 getTileFlightRoundDistance(int x, int y) const;
        int                 getTileDirection(int x, int y) const;
        bool                FindCollision(int x, int y);
        bool                FindTileCollision(int x, int y);

        virtual std::string getName() const { std::terminate(); };
        virtual void        setFacing(int direction) { std::terminate(); };
        virtual void        setAnimation(std::string) { std::terminate(); };
        virtual bool        Render() { std::terminate(); };
        virtual bool        RenderOnMinimap(int x, int y) { std::terminate(); };
        virtual SDL_Color   GetMinimapColor() { std::terminate(); };
        virtual void        Update() { std::terminate(); };
        virtual float       GetMinCollision() { std::terminate(); };
        virtual float       GetMaxCollision() { std::terminate(); };
        virtual int         getAssignment() { std::terminate(); };
        virtual bool        IsTile() { return false; };
        virtual int         getResource() { return false; };
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


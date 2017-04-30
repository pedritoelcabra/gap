#ifndef CCHARACTER_H
#define CCHARACTER_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <CUnit.h>
#include "CVar.h"

class CCharacter : public CUnit
{
    public:

        CCharacter(){}
        CCharacter(int x, int y, std::string name) : CUnit(x,y,name){};
        virtual ~CCharacter(){};

    protected:
    private:
};

#endif // CCHARACTER_H

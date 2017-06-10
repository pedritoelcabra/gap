#ifndef CPLAYER_H
#define CPLAYER_H

#include <CUnit.h>


class CPlayer : public CUnit
{
    public:
        CPlayer(int x_, int y_, std::string myName) : CUnit(x_, y_, myName){};
        virtual ~CPlayer(){};
        virtual void        Destroy();

        void                UpdateAssignment();

    protected:

    private:
};

#endif // CPLAYER_H

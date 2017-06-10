#ifndef CENEMY_H
#define CENEMY_H

#include <CUnit.h>


class CEnemy : public CUnit
{
    public:
        CEnemy(){};
        CEnemy(int x_, int y_, std::string myName) : CUnit(x_, y_, myName){};
        virtual ~CEnemy(){};

        void                UpdateAssignment();
    protected:

    private:
};

#endif // CENEMY_H

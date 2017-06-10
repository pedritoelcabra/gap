#ifndef CATTACKMANAGER_H
#define CATTACKMANAGER_H

#include <vector>
#include <memory>
#include "CLog.h"
#include "CAttack.h"

class CUnit;
typedef std::weak_ptr<CUnit> unit_weak_ptr;

class CAttackManager
{
    public:
        CAttackManager(){};
        virtual ~CAttackManager(){};

        void            AddAttack(unit_weak_ptr att, unit_weak_ptr def, int dmg);
        void            ExecuteAttacks();
    protected:

    private:

        std::vector<CAttack>    Attacks;
};

#endif // CATTACKMANAGER_H

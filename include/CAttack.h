#ifndef CATTACK_H
#define CATTACK_H

#include <memory>

class CUnit;
typedef std::weak_ptr<CUnit> unit_weak_ptr;

class CAttack
{
    public:
        CAttack(){};
        CAttack(unit_weak_ptr att, unit_weak_ptr def, int dmg);
        virtual ~CAttack(){};

        unit_weak_ptr       attacker;
        unit_weak_ptr       defender;
        int                 damage;
};

#endif // CATTACK_H

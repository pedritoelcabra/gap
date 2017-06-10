#include "CAttack.h"

CAttack::CAttack(unit_weak_ptr att, unit_weak_ptr def, int dmg){
    attacker = att;
    defender = def;
    damage = dmg;
}

#include "CAttackManager.h"
#include "CUnit.h"

void CAttackManager::AddAttack(unit_weak_ptr att, unit_weak_ptr def, int dmg){
    Attacks.push_back(CAttack(att, def, dmg));
}

void CAttackManager::ExecuteAttacks(){
    for(auto & a : Attacks){
        if(auto ds = a.defender.lock()){
            if(auto as = a.attacker.lock()){
                ds->DoDamage(a.damage);
            }
        }
    }
    Attacks.clear();
}

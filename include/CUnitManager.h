#ifndef CUnitManager_H
#define CUnitManager_H

#include <memory>
#include <vector>
#include <algorithm>
#include "CGuiObject.h"
#include "CCharacter.h"

typedef std::shared_ptr<CUnit> unit_shared_ptr;
typedef std::weak_ptr<CUnit> unit_weak_ptr;

class CUnitManager
{
    public:
        CUnitManager();
        virtual ~CUnitManager();

        std::vector<unit_weak_ptr>*         Render();
        void                                Update();
        void                                AddNPC(unit_shared_ptr npc);
        unit_weak_ptr                       GetUnit(int target);
        void                                DestroyUnit(int id);


        std::vector<unit_weak_ptr>          FindCollision(int x, int y);
        std::vector<unit_weak_ptr>          FindTileCollision(int x, int y);
        static bool                         sortByYAxis(CGUIObject* a, CGUIObject* b);
    protected:
    private:
        std::vector<unit_shared_ptr>        NPCS;
        int                                 unitCount;
        std::vector<unit_weak_ptr>          renderedUnits;
};

#endif // CUnitManager_H

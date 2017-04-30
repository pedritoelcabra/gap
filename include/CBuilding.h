#ifndef CBUILDING_H
#define CBUILDING_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_gpu.h"
#include "CGuiObject.h"
#include "CBuildingType.h"
#include "CUnit.h"

class CUnit;
class CBuilding;

typedef std::weak_ptr<CUnit> unit_weak_ptr;
typedef std::weak_ptr<CBuilding> build_weak_ptr;

class CBuilding : public CGUIObject
{
    public:
        CBuilding();
        CBuilding(int type, int x, int y, int owner);
        virtual ~CBuilding();

        void                SetId(int id_, build_weak_ptr ptr);
        void                Update();
        bool                Render();
        bool                RenderOnTooltip();
        int                 setX(int x);
        int                 setY(int y);
        int                 getType();
        bool                isInBlockedLocation();
        vec2i               getDoor();
        int                 PopRange();
        int                 MaxPop();
        int                 BuildArea();
        int                 DistributionRange();
        int                 ResourceArea();
        int                 getResource();
        int                 GetTileWidth();
        int                 GetTileHeight();
        void                AddWorker(unit_weak_ptr ptr);
        void                GenerateInhabitant();
        void                RemoveWorker(int id);
        void                RemoveInhabitant(int id);
        unit_weak_ptr       GetIdleInhabitant();
        bool                InRadius(int x, int y);
        bool                HasEnoughWorkers();
        void                Destroy();
        int                 UnderConstruction();
        bool                AddWork(int amount);
        void                AddConnection(build_weak_ptr ptr);
        void                RemoveConnection(int id);
        std::string         GetName();
        std::string         GetDescription();
    protected:

    private:
        bool                isBeingDestroyed;

        int                 owner;
        int                 type;
        int                 popProgress;
        vec2i               door;
        CBuildingType*      typePtr;
        GPU_Rect            clip;
        int                 workToComplete;


        std::vector<unit_weak_ptr>      Workers;
        std::vector<unit_weak_ptr>      Inhabitants;
        std::vector<int>                Inventory;
        std::vector<build_weak_ptr>     ConnectedBuildings;
        build_weak_ptr                  myPtr;

};

#endif // CBUILDING_H

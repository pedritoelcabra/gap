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
        CBuilding(){};
        CBuilding(int type, int x, int y, int owner);
        virtual ~CBuilding(){};

        void                SetId(int id_, build_weak_ptr ptr);
        void                Update();
        bool                Render();
        bool                RenderOnTooltip();
        int                 SetX(int x);
        int                 SetY(int y);
        bool                IsInBlockedLocation();
        void                AddWorker(unit_weak_ptr ptr);
        void                GenerateInhabitant();
        void                RemoveWorker(int id);
        void                RemoveInhabitant(int id);
        unit_weak_ptr       GetIdleInhabitant();
        bool                InRadius(int x, int y);
        bool                HasEnoughWorkers();
        void                Destroy();
        bool                AddWork(int amount);
        void                AddConnection(build_weak_ptr ptr);
        void                AddConnections(std::vector<build_weak_ptr> connections);
        void                RemoveConnection(int id);
        void                ClearConnections();
        int                 AddToInventory(int resource, int amount);
        int                 TakeFromInventory(int resource, int amount);
        void                ApplyMovementCosts();
        int                 GetMaxStorage(int resource);

        std::map<int, int>              GetInventory(){                         return Inventory;};
        std::vector<build_weak_ptr>     GetConnections(){                       return ConnectedBuildings;};
        std::vector<unit_weak_ptr>      GetInhabitants(){                       return Inhabitants;};
        std::vector<unit_weak_ptr>      GetWorkers(){                           return Workers;};
        int                             GetResource(){                          return typePtr->GetResource();};
        int                             GetTileWidth(){                         return GetW() / CScreen::tileWidth;};
        int                             GetTileHeight(){                        return GetH() / CScreen::tileWidth;};
        int                             PopRange(){                             return typePtr->PopRange(); };
        int                             MaxPop(){                               return typePtr->MaxPop(); };
        int                             MaxWorkers(){                           return typePtr->WorkerCount(); };
        int                             BuildArea(){                            return typePtr->BuildArea(); };
        int                             DistributionRange(){                    return typePtr->DistributionRange(); };
        int                             TransportRange(){                       return typePtr->TransportRange(); };
        int                             ResourceArea(){                         return typePtr->ResourceArea(); };
        vec2i                           GetDoor(){                              return door; };
        int                             GetType(){                              return type;};
        int                             UnderConstruction(){                    return workToComplete;};
        std::string                     GetName(){                              return typePtr->GetName();};
        std::string                     GetDescription(){                       return typePtr->GetDescription();};
    protected:

    private:
        bool                isBeingDestroyed = false;

        int                 owner = 0;
        int                 type;
        int                 popProgress = 0;;
        vec2i               door;
        CBuildingType*      typePtr;
        GPU_Rect            clip;
        int                 workToComplete = 0;


        std::vector<unit_weak_ptr>      Workers;
        std::vector<unit_weak_ptr>      Inhabitants;
        std::map<int, int>              Inventory;
        std::vector<build_weak_ptr>     ConnectedBuildings;
        build_weak_ptr                  myPtr;

};

#endif // CBUILDING_H

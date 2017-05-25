#ifndef CBUILDING_H
#define CBUILDING_H

#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_gpu.h"
#include "CGuiObject.h"
#include "CBuildingType.h"
#include "CUnit.h"

#include "CTransportTask.h"
#include "CTaskManager.h"

class CUnit;
class CBuilding;
class CTransportTask;

typedef std::weak_ptr<CUnit> unit_weak_ptr;
typedef std::weak_ptr<CBuilding> build_weak_ptr;
typedef std::weak_ptr<CTransportTask> task_weak_ptr;

class CBuilding : public CGUIObject
{
    public:
        CBuilding(){};
        CBuilding(int type, int x, int y, int owner);
        virtual ~CBuilding(){};

        void                SetId(int id_, build_weak_ptr ptr);
        void                Update();
        void                UpdateTransportTasks();
        void                MatchTransportTasks();
        task_weak_ptr       FindPlaceToDropOff(int resource);
        void                UpdateNeededGoods();
        bool                Render();
        bool                RenderOnTooltip();
        bool                RenderInPosition(GPU_Rect* destBox);
        int                 SetX(int x);
        int                 SetY(int y);
        bool                IsInBlockedLocation();
        void                AddWorker(unit_weak_ptr ptr);
        void                GenerateInhabitant();
        void                RemoveWorker(int id);
        void                RemoveInhabitant(int id);
        unit_weak_ptr       GetIdleInhabitant();
        bool                HasEnoughWorkers();
        bool                HasWorkToDo();
        void                Destroy();
        bool                AddWork(int amount, bool setUp = true);
        void                AddIncoming(task_weak_ptr ptr);
        void                RemoveIncoming(int id);
        void                AddOutgoing(task_weak_ptr ptr);
        void                RemoveOutgoing(int id);
        void                AddConnection(build_weak_ptr ptr);
        void                AddConnections(std::vector<build_weak_ptr> connections);
        void                RemoveConnection(int id);
        void                ClearConnections();
        int                 AddToInventory(int resource, int amount);
        int                 TakeFromInventory(int resource, int amount);
        void                ApplyMovementCosts(bool destroy = false);
        int                 GetMaxStorage(int resource, bool excludingOrders = false);
        int                 IncomingByResource(int res, bool onlyPending = false);
        int                 OutgoingByResource(int res, bool onlypending = false);
        task_weak_ptr       FindConnectedTask(int x, int y);
        bool                HasBuildingResources();
        vec2i               GetRandomPassableTile();
        bool                IsValidWorkLocation(int x, int y);
        bool                CanProduce();
        bool                DoProduce();
        int                 StartProduction();
        CRecipe*            AvailableRecipe();
        void                ConnectToNearestTown(bool forceReconnect = false);
        void                AddResourceOffer(build_weak_ptr sourcePtr, int res);
        void                AddResourceRequest(build_weak_ptr sourcePtr, int res, int prio);
        bool                InventoryAvailable();
        void                UseInventory();
        int                 MaxAssignedWorkers(int newMax = -1);

        std::vector<build_weak_ptr>*    GetConnections();

        std::map<int, int>              GetInventory(){                         return Inventory;};
        std::vector<unit_weak_ptr>      GetInhabitants(){                       return Inhabitants;};
        std::vector<CRecipe>*           GetRecipes(){                           return &Recipes;};
        std::vector<unit_weak_ptr>      GetWorkers(){                           return Workers;};
        std::vector<task_weak_ptr>      GetOutgoing(){                          return Outgoing;};
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
        int                             DoorX(){                                return door.first; };
        int                             DoorY(){                                return door.second; };
        int                             GetType(){                              return type;};
        int                             UnderConstruction(){                    return workToComplete;};
        std::string*                    GetName(){                              return typePtr->GetName();};
        std::string*                    GetDescription(){                       return typePtr->GetDescription();};
        bool                            IsRoad(){                               return typePtr->IsRoad();};
        int                             ConsumesResource(int res_ = 0){         return typePtr->ConsumesResource(res_);};
        int                             ProducesResource(int res_ = 0){         return typePtr->ProducesResource(res_);};

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
        CRecipe*            currentProduction = nullptr;
        int                 currentProductionStage = 0;
        int                 currentProductionCooldown = 0;
        int                 resourcePrio = 10;
        int                 lastItemPickedUp = 0;
        int                 maxAssignedWorkers;


        std::vector<task_weak_ptr>      Outgoing;
        std::vector<task_weak_ptr>      Incoming;
        std::vector<unit_weak_ptr>      Workers;
        std::vector<unit_weak_ptr>      Inhabitants;
        std::map<int, int>              Inventory;
        std::vector<build_weak_ptr>     ConnectedBuildings;
        std::vector<vec2i>              PassableTiles;
        build_weak_ptr                  myPtr;
        build_weak_ptr                  myTown;
        std::vector<CRecipe>            Recipes;

        std::map<int, int>                              NeededGoods;

        std::map<int, std::vector<task_weak_ptr> >      RequestedGoods;
        std::map<int, std::vector<task_weak_ptr> >      OfferedGoods;
        std::vector<task_weak_ptr>                      AvailableTasks;

};

#endif // CBUILDING_H

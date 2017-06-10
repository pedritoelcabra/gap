#ifndef CUNIT_H
#define CUNIT_H

#include <vector>
#include <CGUIObject.h>
#include <CAction.h>
#include <CBuilding.h>
#include <CGood.h>
#include "CTile.h"
#include "CTransportTask.h"

class CUnit;
class CBuilding;
class CTile;
class CTransportTask;


typedef std::weak_ptr<CUnit> unit_weak_ptr;
typedef std::weak_ptr<CBuilding> build_weak_ptr;
typedef std::weak_ptr<CTile> tile_weak_ptr;
typedef std::weak_ptr<CTransportTask> task_weak_ptr;

class CUnit : public CGUIObject
{
    public:

        CUnit(){};
        CUnit(int x, int y, std::string name) ;
        virtual ~CUnit(){};

        bool                PreRender();
        bool                Render();
        bool                RenderInPosition(GPU_Rect* destBox);
        void                SetAnimation(std::string);
        void                Update();

        bool                IsIdle(){                           return isIdle;};

        void                AddAction(CAction action_);
        void                ClearActions();
        bool                MoveTo(int x, int y, int stopShort = 0);
        bool                MoveNextTo(int x, int y, bool diagonal = true);
        int                 ExecuteAction(CAction action_);
        int                 FinishAction(CAction action_);
        bool                MoveToAdyacent(int x_, int y_);
        void                StopMovement();
        void                Idle(int time, bool addOnly = false);
        virtual void        Destroy();
        void                CarryItem(int resource){    carriedItem = resource;};
        int                 GetCarriedItem(bool takeIt = false);

        float               GetMinCollision(){          return minCollision;};
        float               GetMaxCollision(){          return maxCollision;};
        build_weak_ptr      GetHome(){                  return homeBuildingPtr;};
        build_weak_ptr      GetWorkplace(){             return workBuildingPtr;};
        int                 GetAssignment() {           return assignment; };
        int                 GetBusyTime(){              return busyTime; };
        int                 GetActionCount(){           return ActionQueue.size(); };
        std::weak_ptr<CUnit>GetPtr(){                   return myPtr;};
        std::string*        GetName() {                 return &name;};
        std::string*        GetThought() {              return &thought;};
        void                SetSpeed(int speed){        baseSpeed = speed; };

        void                SetIdleAssignment();
        void                SetFollowAssignment(unit_weak_ptr ptr);

        virtual void                SetTraderAssignment(){                          std::terminate(); };
        virtual bool                SetTransportAssignment(task_weak_ptr ptr){      std::terminate(); };
        virtual void                UpdateAssignment(){};
        virtual void                CancelTransportTask(){};
        virtual void                FillTraderInventory(build_weak_ptr src, build_weak_ptr dest){};

        void                SetHome(build_weak_ptr ptr){        if(auto s = ptr.lock()){        homeBuildingPtr = ptr;    }};
        void                SetFacing(int direction);
        void                SetMoving(bool);
        void                SetId(int id_, std::weak_ptr<CUnit> myPtr_){    id = id_;    myPtr = myPtr_;};
        void                SetSkill(int skill){        skillLvl = skill; };
        int                 GetSkill(){                 return skillLvl; };

        std::map<int, int>*             GetInventory(){                         return &Inventory;};

        int                 Owner(int newOwner = 0){       if(newOwner > 0){ owner = newOwner; UpdateNearbyUnits(); } return owner; };
        void                UpdateNearbyUnits();
        void                DoDamage(int amount);

    protected:

        int                 SetFrameCount();
        int                 SetFrameOffset();
        int                 SetFrameSpeed();
        int                 BaseOffset();
        int                 FrameSpeed();
        void                ResetFrames();

        std::string         name;
        std::string         thought;
        unit_weak_ptr       myPtr;
        int                 currentAnimation = 7;
        Uint32              currentFrame = 0;
        Uint32              currentFrameSpeed;
        Uint32              currentFrameStarted = 0;
        Uint32              frameCount;
        Uint32              frameOffset;
        int                 facing = 2;
        int                 moveDirection = 4;
        bool                moving = false;
        float               currentSpeed;
        float               carryOverPixels;
        int                 xoff;
        int                 yoff;
        float               currentX;
        float               currentY;

        int                 busyTime = 0;
        bool                isIdle;
        float               minCollision = 0.0f;
        float               maxCollision = 3.0f;

        int                 assignment;
        int                 skillLvl = 0;

        int                 owner = 0;

        int                 targetUnit;
        int                 targetBuilding;
        int                 targetObject;
        Coord               targetTile;
        bool                hasTargetTile;
        unit_weak_ptr       targetUnitPtr;
        build_weak_ptr      targetBuildingPtr;
        build_weak_ptr      homeBuildingPtr;
        build_weak_ptr      workBuildingPtr;
        tile_weak_ptr       resourceTilePtr;
        task_weak_ptr       taskPtr;

        CAction             currentAction;

        std::vector<CAction>    ActionQueue;
        std::map<int, int>      Inventory;
        int                     inventorySize = 10;
        int                     carriedItem = 0;
        GPU_Rect                itemBox;

        std::vector<unit_weak_ptr>          NearbyUnits;
        std::vector<unit_weak_ptr>          NearbyHostiles;

        int                 baseSpeed = 16;
        Uint32              moveSpeed = baseSpeed;
        int                 detectionDistance = 20;
        int                 maxHealth = 1000;
        int                 currentHealth = maxHealth;
        int                 armor = 100;
        int                 attackDamage = 100;
        int                 attackSpeed = 100;

        int                 healthBarWidth;
        int                 healthBarMaxWidth;
        SDL_Color           healthBarColor;
};

#endif // CUNIT_H

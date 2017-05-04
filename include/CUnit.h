#ifndef CUNIT_H
#define CUNIT_H

#include <vector>
#include <CGUIObject.h>
#include <CAction.h>
#include <CBuilding.h>
#include <CGood.h>
#include "CTile.h"

class CUnit;
class CBuilding;
class CTile;


typedef std::weak_ptr<CUnit> unit_weak_ptr;
typedef std::weak_ptr<CBuilding> build_weak_ptr;
typedef std::weak_ptr<CTile> tile_weak_ptr;

class CUnit : public CGUIObject
{
    public:
        enum EEmotes { idle, building, nothingToBuild, woodcutting, noWood };

        CUnit(){};
        CUnit(int x, int y, std::string name) ;
        virtual ~CUnit();

        bool                Render();
        void                SetAnimation(std::string);
        void                Update();

        bool                IsIdle();

        void                AddAction(CAction action_);
        void                ClearActions();
        bool                MoveTo(int x, int y, int stopShort = 0);
        bool                MoveNextTo(int x, int y, bool diagonal = true);
        int                 ExecuteAction(CAction action_);
        int                 FinishAction(CAction action_);
        bool                MoveToAdyacent(int x_, int y_);
        void                StopMovement();
        void                Idle(int time);
        void                UpdateAssignment();
        void                GatherResource(int resource);
        void                Destroy();
        void                CarryItem(int resource);
        int                 GetCarriedItem(bool takeIt = false);

        float               GetMinCollision(){          return minCollision;};
        float               GetMaxCollision(){          return maxCollision;};
        build_weak_ptr      GetHome(){                  return homeBuildingPtr;};
        build_weak_ptr      GetWorkplace(){             return workBuildingPtr;};
        int                 GetAssignment() {           return assignment; };
        int                 GetBusyTime(){              return busyTime; };
        std::weak_ptr<CUnit>GetPtr(){                   return myPtr;};
        std::string         GetName() const{            return name;};
        std::string         GetThought() const{         return thought;};
        std::string         GetAssignmentName() const;
        float               GetMoveSpeed();

        void                SetIdleAssignment();
        void                SetFollowAssignment(unit_weak_ptr ptr);
        void                SetGatherAssignment(build_weak_ptr ptr);
        void                SetBuildAssignment(build_weak_ptr ptr);
        void                SetHome(build_weak_ptr ptr);
        void                SetFacing(int direction);
        void                SetMoving(bool);
        void                SetId(int id_, std::weak_ptr<CUnit> myPtr_);


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
        int                 currentAnimation;
        Uint32              currentFrame;
        Uint32              currentFrameSpeed;
        Uint32              currentFrameStarted;
        Uint32              frameCount;
        Uint32              frameOffset;
        int                 facing;
        int                 moveDirection;
        bool                moving;
        Uint32              moveSpeed;
        float               currentSpeed;
        float               carryOverPixels;
        int                 baseSpeed;
        int                 xoff;
        int                 yoff;

        int                 busyTime;
        bool                isIdle;
        float               minCollision;
        float               maxCollision;

        int                 assignment;

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

        CAction             currentAction;

        std::vector<CAction>    ActionQueue;
        std::map<int, int>      Inventory;
        int                     carriedItem;

        static const int        resourceRadius = 20;
};

#endif // CUNIT_H
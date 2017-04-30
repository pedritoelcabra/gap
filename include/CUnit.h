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
        void                setAnimation(std::string);
        void                Update();
        std::string         getName() const;
        void                setFacing(int direction);
        void                setMoving(bool);
        void                setId(int id_, std::weak_ptr<CUnit> myPtr_);
        std::weak_ptr<CUnit>       GetPtr();

        bool                IsIdle();
        int                 GetBusyTime();

        void                AddAction(CAction action_);
        void                ClearActions();
        bool                MoveTo(int x, int y, int stopShort = 0);
        bool                MoveNextTo(int x, int y, bool diagonal = true);
        int                 ExecuteAction(CAction action_);
        int                 FinishAction(CAction action_);
        bool                MoveToAdyacent(int x_, int y_);
        void                StopMovement();
        void                Idle(int time);
        float               getMoveSpeed();
        float               GetMinCollision();
        float               GetMaxCollision();

        void                SetIdleAssignment();
        void                SetFollowAssignment(unit_weak_ptr ptr);
        void                SetGatherAssignment(build_weak_ptr ptr);
        void                SetBuildAssignment(build_weak_ptr ptr);
        void                UpdateAssignment();
        int                 getAssignment();
        void                GatherResource(int resource);
        void                DropBackpack();
        void                SetHome(build_weak_ptr ptr);

        void                Destroy();

    protected:
        std::string         name;
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

        int                 setFrameCount();
        int                 setFrameOffset();
        int                 setFrameSpeed();
        int                 baseOffset();
        int                 frameSpeed();
        void                resetFrames();

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
        std::vector<int>        BackPack;

        static const int        resourceRadius = 20;
};

#endif // CUNIT_H

#ifndef CWORKER_H
#define CWORKER_H

#include "CUnit.h"

class CWorker : public CUnit
{
    public:
        CWorker(){};
        CWorker(int x_, int y_, std::string myName) : CUnit(x_, y_, myName){};
        virtual ~CWorker(){};

        void                SetTraderAssignment();
        bool                SetTransportAssignment(task_weak_ptr ptr);
        void                UpdateAssignment();
        void                UpdateBuildAssignment();
        void                UpdateGatherAssignment();
        void                UpdateFollowAssignment();
        void                UpdateIdleAssignment();
        void                UpdateProductionAssignment();
        void                UpdateTransportAssignment();
        void                UpdateTraderAssignment();
        void                CancelTransportTask();
        void                FillTraderInventory(build_weak_ptr src, build_weak_ptr dest);
    protected:

    private:
};

#endif // CWORKER_H

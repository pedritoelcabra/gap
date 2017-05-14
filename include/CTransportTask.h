
#ifndef CTRANSPORTTASK_H
#define CTRANSPORTTASK_H

#include <CBuilding.h>
#include <CUnit.h>

class CUnit;
class CBuilding;

typedef std::weak_ptr<CUnit> unit_weak_ptr;
typedef std::weak_ptr<CBuilding> build_weak_ptr;

class CTransportTask
{

    public:

        CTransportTask(build_weak_ptr pickUp_, build_weak_ptr dropOff_, int resource_, int prio_ ){
            pickUp = pickUp_;
            dropOff = dropOff_;
            resource = resource_;
            prio = prio_;
        };
        virtual ~CTransportTask(){};

        void                AssignPorter(unit_weak_ptr porter_){ porter = porter_; };
        void                AssignOrigin(build_weak_ptr pickUp_){ pickUp = pickUp_; };
        build_weak_ptr      GetPickUp() { return pickUp ;};
        build_weak_ptr      GetDropOff() { return dropOff ;};
        unit_weak_ptr       GetPorter() { return porter ;};
        void                SetId(int id_) { id = id_;};
        int                 GetId(){ return id;};
        int                 GetPrio(){ return prio;};
        int                 GetResource(){ return resource;};
        bool                GetCompleted(){ return completed;};
        void                MarkComplete(){ completed = true; };

    private:

        bool                completed = false;
        int                 id;
        int                 resource;
        int                 prio;
        build_weak_ptr      pickUp;
        build_weak_ptr      dropOff;
        unit_weak_ptr       porter;

};

#endif // CTRANSPORTTASK_H

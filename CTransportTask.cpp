#include "CTransportTask.h"

CTransportTask::CTransportTask(build_weak_ptr pickUp_, build_weak_ptr dropOff_, int resource_, int prio_ ){
    pickUp = pickUp_;
    if(auto s = pickUp.lock()){
        door = s->GetDoor();
    }
    dropOff = dropOff_;
    resource = resource_;
    prio = prio_;
};

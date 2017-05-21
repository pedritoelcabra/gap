#include "CTransportTask.h"
#include "CGame.h"

extern CGame GAP;

CTransportTask::CTransportTask(build_weak_ptr pickUp_, build_weak_ptr dropOff_, int resource_, int prio_ ){
    pickUp = pickUp_;
    if(auto s = pickUp.lock()){
        door = s->GetDoor();
    }
    dropOff = dropOff_;
    resource = resource_;
    prio = prio_;
};

bool CTransportTask::CheckPath(){
    if(auto ps = pickUp.lock()){
        if(auto ds = dropOff.lock()){
            vec2i ddoor = ds->GetDoor();
            CoordList path = GAP.Pathfinder.FindPath(
                Coord( door.first, door.second ),
                Coord( ddoor.first, ddoor.second ),
                0.0f, CScreen::flatMoveCost
            );
            if(path.size() > 0){
                return true;
            }
        }
    }
    return false;
}

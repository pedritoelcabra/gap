#include "CEnemy.h"


void CEnemy::UpdateAssignment(){
    int distance;
    if(auto s = targetUnitPtr.lock()){
        distance = GetTileFlightSquareDistance(s->GetTileX(), s->GetTileY());
        if(distance == 1){
            AddAction(CAction(CAction::doSlash, 1, 1));
            return;
        }
        if(distance > detectionDistance){
            targetUnitPtr.reset();
            return;
        }
        if(distance < 5){
            MoveTo(s->GetTileX(), s->GetTileY());
        }else{
            MoveTo(s->GetTileX(), s->GetTileY(), distance / 3);
        }
        return;
    }
    int bestDist = 999;
    for(auto w : NearbyHostiles){
        if(auto s = w.lock()){
            distance = GetTileFlightRoundDistance(s->GetTileX(), s->GetTileY());
            if(distance < bestDist){
                bestDist = distance;
                targetUnitPtr = w;
            }
        }
    }
    if(bestDist == 999){
        Idle(60);
    }
}

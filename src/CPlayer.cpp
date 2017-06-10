#include "CPlayer.h"
#include "CGame.h"

extern CGame GAP;

void CPlayer::Destroy(){
    GAP.Exit();
}

void CPlayer::UpdateAssignment(){
    int distance;
    if(auto s = targetUnitPtr.lock()){
        distance = GetTileFlightSquareDistance(s->GetTileX(), s->GetTileY());
        if(distance == 1){
            AddAction(CAction(CAction::doSlash, 1, 1));
            return;
        }
        targetUnitPtr.reset();
    }
    int bestDist = 2;
    for(auto w : NearbyHostiles){
        if(auto s = w.lock()){
            distance = GetTileFlightRoundDistance(s->GetTileX(), s->GetTileY());
            if(distance < bestDist){
                bestDist = distance;
                targetUnitPtr = w;
            }
        }
    }
}


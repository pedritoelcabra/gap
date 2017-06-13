#include "CEnemy.h"

CEnemy::CEnemy(int x_, int y_, std::string myName) : CUnit(x_, y_, myName){

    std::string fileName = "defines/units/";
    fileName.append(myName);
    fileName.append(".txt");
	std::ifstream is_file(fileName);
	std::string line = "";

    if (is_file.is_open()){
        while( std::getline(is_file, line) ) {
            std::istringstream is_line(line);
            std::string key = "";
            std::string value = "";
            std::getline(is_line, key, '=');
            std::getline(is_line, value);
            LoadLine(key, value);
        }
	}
}

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
            if(distance < bestDist && distance < detectionDistance){
                bestDist = distance;
                targetUnitPtr = w;
            }
        }
    }
    if(bestDist == 999){
        if(rand() % 2 < 1){
            MoveNextTo(tileX, tileY);
        }
        Idle(120, true);
    }
}

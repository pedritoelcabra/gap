
#include "CGame.h"

build_weak_ptr CGame::Building(int id){
    return BuildingManager.GetBuilding(id);
}

build_weak_ptr CGame::BuildingAt(int x, int y){
    return BuildingManager.GetBuildingAt(x, y);
}

CBuildingType* CGame::BuildingType(int type){
    return BuildingManager.GetBuildingType(type);
}

tile_weak_ptr CGame::Tile(int x, int y){
    return ChunkManager.GetTile(x,y);
}

unit_weak_ptr CGame::Unit(int target){
    return UnitManager.GetUnit(target);
}

std::vector<unit_weak_ptr> CGame::UnitsAtTile(int x, int y){
    if(auto s = Tile(x,y).lock()){
        return s->UnitsAtTile();
    }
    std::vector<unit_weak_ptr> emptyVector;
    return emptyVector;
}

int CGame::GetSeed() const{
    return seed;
}

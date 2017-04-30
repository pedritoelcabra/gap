#include "CUnitManager.h"
#include "CGame.h"

extern CGame GAP;

CUnitManager::CUnitManager(){
    //ctor
    unitCount = 0;
}

CUnitManager::~CUnitManager(){
    //dtor
}

std::vector<unit_weak_ptr>* CUnitManager::Render(){

//    renderedUnits.clear();
//
//    renderedUnits.push_back(&GAP.Player);
//
//    for(auto &e : NPCS)
//    {
//        renderedUnits.push_back(&e.second);
//    }
    return &renderedUnits;
}

void CUnitManager::Update(){

    for(auto e : NPCS)
    {
        e->Update();
    }
}

bool CUnitManager::sortByYAxis(CGUIObject* a, CGUIObject* b) {

    if(a->getY() == b->getY()){
        return a->getZ() < b->getZ();
    }
    return a->getY() < b->getY();
}

void CUnitManager::AddNPC(unit_shared_ptr npc){
    unitCount++;
    npc->setId(unitCount, unit_weak_ptr(npc));
    tile_weak_ptr ptr = GAP.ChunkManager.GetTile(npc->getTileX(), npc->getTileY());
    if(auto s = ptr.lock()){
        s->AddUnit(unit_weak_ptr(npc));
        NPCS.push_back(npc);
        return;
    }
    CLog::Write("Could not register unit with tile");
}

std::vector<unit_weak_ptr>  CUnitManager::FindCollision(int x, int y){

    std::vector<unit_weak_ptr> npcs;
    for(auto e : NPCS)
    {
        if(e->FindCollision(x, y)){
            npcs.push_back( unit_weak_ptr(e) );
        }
    }
    return npcs;
}

std::vector<unit_weak_ptr>  CUnitManager::FindTileCollision(int x, int y){

    std::vector<unit_weak_ptr> npcs;
    for(auto e : NPCS)
    {
        if(e->FindTileCollision(x, y)){
            npcs.push_back( unit_weak_ptr(e) );
        }
    }
    return npcs;
}

unit_weak_ptr CUnitManager::GetUnit(int target){
    return unit_weak_ptr();
}

void CUnitManager::DestroyUnit(int id){
    std::vector<unit_shared_ptr>::iterator iter = NPCS.begin();
    while (iter != NPCS.end())
    {
        if((*iter)->GetId() == id){
            NPCS.erase(iter);
            return;
        }
        iter++;
    }
}


#include "CBuilding.h"
#include "CGame.h"

extern CGame GAP;

CBuilding::CBuilding(){
    isBeingDestroyed = false;
}

CBuilding::CBuilding(int type, int x, int y, int owner){
    this->tileX = x;
    this->x = (x * CScreen::tileWidth) ;
    this->tileY = y;
    this->y = (y * CScreen::tileWidth) ;
    this->type = type;
    this->owner = owner;
    typePtr = GAP.BuildingManager.GetBuildingType(type);
    box.w = typePtr->GetW();
    box.h = typePtr->GetH();
    workToComplete = typePtr->GetCost(CGood::work);
    clip = typePtr->GetClip();
    if(workToComplete){
        clip.y = clip.h;
    }
    box.x = this->x;
    box.y = this->y;
    popProgress = 0;
    door = typePtr->GetDoor();
    door.first += tileX;
    door.second += tileY;
    isBeingDestroyed = false;
    for(auto p : CGood::GetResources() ){
        Inventory[p.first] = 0;
    }
    //ctor
}

CBuilding::~CBuilding(){
    //dtor
}

void CBuilding::SetId(int id_, build_weak_ptr ptr){
    id = id_;
    myPtr = ptr;
}

bool CBuilding::Render(){

    GAP.TextureManager.DrawTextureGL(typePtr->GetName(), &clip, &box);
    return 1;
}

bool CBuilding::RenderOnTooltip(){
    GPU_Image* tex = GAP.TextureManager.GetTexture(typePtr->GetName());
    if(IsInBlockedLocation()){
        GPU_SetRGBA(tex, 255, 100, 100, 100);
    }else{
        GPU_SetRGBA(tex, 255, 255, 255, 100);
    }

    Render();

    GPU_SetRGBA(tex, 255, 255, 255, 255);

    return 1;
}

int CBuilding::SetX(int x){
    this->tileX = x;
    this->x = x * CScreen::tileWidth;
    box.x = this->x;
    door.first = (x + typePtr->GetDoor().first);
    return x;
}

int CBuilding::SetY(int y){
    this->tileY = y;
    this->y = y * CScreen::tileWidth;
    box.y = this->y;
    door.second = (y + typePtr->GetDoor().second);
    return y;
}

bool CBuilding::IsInBlockedLocation(){
    for(int i = 0; i < GetTileHeight(); i++ ){
        for(int k = 0; k < GetTileWidth(); k++ ){
            if(GAP.Pathfinder.GetCost(tileX + k, tileY + i) >= 3){
                return true;
            }
            if(GAP.UnitManager.FindTileCollision(tileX + k, tileY + i).size() > 0){
                return true;
            }
        }
    }
    return false;
}

void CBuilding::Update(){
    if(typePtr->MaxPop() > static_cast<int>(Inhabitants.size())){
        popProgress++;
        if(popProgress >= typePtr->PopCost()){
            popProgress = 0;
            GenerateInhabitant();
        }
    }
}

void CBuilding::GenerateInhabitant(){
    unit_shared_ptr is = std::make_shared<CUnit>(door.first, door.second, "player");
    unit_weak_ptr iw = unit_weak_ptr(is);

    GAP.UnitManager.AddNPC(is);
    is->SetHome(myPtr);
    Inhabitants.push_back(iw);
}

void CBuilding::AddWorker(unit_weak_ptr ptr){
    Workers.push_back(ptr);
}

void CBuilding::RemoveWorker(int id){
    if(isBeingDestroyed){
        return;
    }
    std::vector<unit_weak_ptr>::iterator iter = Workers.begin();
    while (iter != Workers.end())
    {
        if(auto s = (*iter).lock()){
            if(s->GetId() == id){
                Workers.erase(iter);
                return;
            }
        }
        iter++;
    }
    CLog::Write("Trying to remove nonexistant unit from building workers?");
}

void CBuilding::AddConnection(build_weak_ptr ptr){
    ConnectedBuildings.push_back(ptr);
}

void CBuilding::AddConnections(std::vector<build_weak_ptr> connections){
    ConnectedBuildings.insert( ConnectedBuildings.end(), connections.begin(), connections.end() );
    RemoveConnection(id);
}

void CBuilding::RemoveConnection(int id_){
    if(isBeingDestroyed){
        return;
    }
    std::vector<build_weak_ptr>::iterator iter = ConnectedBuildings.begin();
    while (iter != ConnectedBuildings.end())
    {
        if(auto s = (*iter).lock()){
            if(s->GetId() == id_){
                ConnectedBuildings.erase(iter);
                return;
            }
        }
        iter++;
    }
    CLog::Write("Trying to remove nonexistant building from building connections?");
}

void CBuilding::ClearConnections(){
    ConnectedBuildings.clear();
}

void CBuilding::RemoveInhabitant(int id){
    if(isBeingDestroyed){
        return;
    }
    std::vector<unit_weak_ptr>::iterator iter = Inhabitants.begin();
    while (iter != Inhabitants.end())
    {
        if(auto s = (*iter).lock()){
            if(s->GetId() == id){
                Inhabitants.erase(iter);
                return;
            }
        }
        iter++;
    }
    CLog::Write("Trying to remove nonexistant unit from building inhabitants?");
}

bool CBuilding::InRadius(int x, int y){
    if(typePtr->PopRange() > 0){
        if(GetTileFlightRoundDistance(x, y) <= typePtr->PopRange()){
            return true;
        }
    }
    return false;
}

unit_weak_ptr CBuilding::GetIdleInhabitant(){
    for(auto e : Inhabitants)    {
        if(auto s = e.lock()){
            if(s->GetAssignment() == CAction::idleAssignment){
                return e;
            }
        }
    }
    unit_weak_ptr emptyPtr;
    return emptyPtr;
}

bool CBuilding::HasEnoughWorkers(){
    return static_cast<int>(Workers.size()) >= typePtr->WorkerCount() ;
}

void CBuilding::Destroy(){
    isBeingDestroyed = true;
    for(auto e : Inhabitants)    {
        if(auto s = e.lock()){
            s->Destroy();
        }
    }
}

bool CBuilding::AddWork(int amount){
    workToComplete -= amount;
    if(workToComplete <= 0){
        clip.y = 0;
        workToComplete = 0;
        ApplyMovementCosts();
        return true;
    }
    return false;
}

void CBuilding::AddToInventory(int resource, int amount){
    Inventory.at(resource) += amount;
}

int CBuilding::TakeFromInventory(int resource, int amount){
    if(Inventory.at(resource) >= amount){
        Inventory.at(resource) -= amount;
        return amount;
    }
    int rest = Inventory.at(resource);
    Inventory.at(resource) = 0;
    return rest;
}

void CBuilding::ApplyMovementCosts(){
    auto layout = typePtr->GetLayout();
    for(int i = 0; i < GetTileHeight(); i++ ){
        for(int k = 0; k < GetTileWidth(); k++ ){
            if(layout.at(i).at(k) < CScreen::flatMoveCost && workToComplete > 0){
                continue;
            }
            GAP.Pathfinder.SetCost(tileX + k, tileY + i, layout.at(i).at(k));
        }
    }
}


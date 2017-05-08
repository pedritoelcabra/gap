#include "CBuilding.h"
#include "CGame.h"
#include "CTaskManager.h"
#include "Templates.h"

extern CGame GAP;

CBuilding::CBuilding(int type_, int x_, int y_, int owner_){
    tileX = x_;
    box.x = x = (x_ * CScreen::tileWidth) ;
    tileY = y_;
    box.y = y = (y_ * CScreen::tileWidth) ;
    type = type_;
    owner = owner_;
    typePtr = GAP.BuildingManager.GetBuildingType(type);
    box.w = typePtr->GetW();
    box.h = typePtr->GetH();
    workToComplete = typePtr->GetCost(CGood::work);
    clip = typePtr->GetClip();
    if(workToComplete){
        clip.y = clip.h;
    }
    door = typePtr->GetDoor();
    door.first += tileX;
    door.second += tileY;
    for(auto p : CGood::GetResources() ){
        Inventory[p.first] = 0;
    }
}

void CBuilding::SetId(int id_, build_weak_ptr ptr){
    id = id_;
    myPtr = ptr;
}

bool CBuilding::Render(){
    if(workToComplete){
        clip.y = clip.h;
        GAP.TextureManager.DrawTextureGL(typePtr->GetName(), &clip, &box);
        int parts = clip.h / typePtr->BuildCost(CGood::work);
        int oldBoxY = box.y;
        clip.y = parts * workToComplete;
        box.y = box.y + clip.y;
        box.h = clip.h = parts * (typePtr->BuildCost(CGood::work) - workToComplete);
        GAP.TextureManager.DrawTextureGL(typePtr->GetName(), &clip, &box);
        clip = typePtr->GetClip();
        box.h = typePtr->GetH();
        box.y = oldBoxY;
        return 1;
    }
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
            if(!typePtr->IsRoad()){
                if(GAP.UnitManager.FindTileCollision(tileX + k, tileY + i).size() > 0){
                    return true;
                }
            }
        }
    }
    return false;
}

bool CBuilding::HasBuildingResources(){
    for(auto const &c : typePtr->GetBuildCosts()){
        if( c.first != CGood::work && Inventory.at(c.first) < c.second){
            return false;
        }
    }
    return true;
}

void CBuilding::Update(){
    if(workToComplete){
        return;
    }
    DeleteUnusedWeak(&Workers);
    if(typePtr->MaxPop() > static_cast<int>(Inhabitants.size())){
        popProgress++;
        if(popProgress >= typePtr->PopCost()){
            popProgress = 0;
            GenerateInhabitant();
        }
    }
    int outgoinTasks;
    for (auto const& item : Inventory){
        outgoinTasks = OutgoingByResource(item.first);
        while(item.second > outgoinTasks){
            build_weak_ptr w = FindNearestStorage(item.first);
            if(auto s = w.lock()){
                task_shared_ptr ts = std::make_shared<CTransportTask>(myPtr, w, item.first);
                task_weak_ptr tw = task_weak_ptr(ts);
                GAP.TaskManager.AddTask(ts);
                s->AddIncoming(tw);
                AddOutgoing(tw);
            }
            outgoinTasks++;
        }
    }
}

void CBuilding::GenerateInhabitant(){
    unit_shared_ptr is = std::make_shared<CUnit>(door.first, door.second, "player");
    unit_weak_ptr iw = unit_weak_ptr(is);

    GAP.UnitManager.AddNPC(is);
    is->SetHome(myPtr);
    is->SetIdleAssignment();
    Inhabitants.push_back(iw);
}

void CBuilding::AddIncoming(task_weak_ptr ptr){
    Incoming.push_back(ptr);
}

void CBuilding::RemoveIncoming(int id){
    if(isBeingDestroyed){
        return;
    }
    std::vector<task_weak_ptr>::iterator iter = Incoming.begin();
    while (iter != Incoming.end())    {
        if(auto s = (*iter).lock()){
            if(s->GetId() == id){
                Incoming.erase(iter);
                return;
            }
        }
        iter++;
    }
}

void CBuilding::AddOutgoing(task_weak_ptr ptr){
    Outgoing.push_back(ptr);
}

void CBuilding::RemoveOutgoing(int id){
    if(isBeingDestroyed){
        return;
    }
    std::vector<task_weak_ptr>::iterator iter = Outgoing.begin();
    while (iter != Outgoing.end())    {
        if(auto s = (*iter).lock()){
            if(s->GetId() == id){
                Outgoing.erase(iter);
                return;
            }
        }
        iter++;
    }
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

bool CBuilding::HasWorkToDo(){
    if(ResourceArea() > 0 && GAP.ChunkManager.GetResources(GetResource(), ResourceArea(), DoorX(), DoorY()).size() > 0){
        return true;
    }
    if(CanProduce()){
        return true;
    }
    return false;
}

bool CBuilding::CanProduce(){
    if(AvailableRecipe() == nullptr){
        return false;
    }
    return true;
}

CRecipe* CBuilding::AvailableRecipe(){
    if(!ConsumesResource(0)){
        return nullptr;
    }
    for(auto const & recipe : Recipes){
        bool canProduce = true;
        for(auto const &c : recipe->GetInput()){
            if( c.first != CGood::work && Inventory.at(c.first) < c.second){
                canProduce = false;
            }
        }
        for(auto const &c : recipe->GetOutput()){
            if( GetMaxStorage(c.first) < c.second){
                canProduce = false;
            }
        }
        if(canProduce){
            return recipe;
        }
    }
    return nullptr;
}

int CBuilding::StartProduction(){
    int timeRequired = 0;
    currentProduction = AvailableRecipe();
    if(currentProduction == nullptr){
        return timeRequired;
    }
    for(auto const &c : currentProduction->GetInput()){
        if( c.first == CGood::work ){
            timeRequired = c.second * CScreen::buildingUpdateFrequency;
        }else{
            TakeFromInventory(c.first, c.second);
        }
    }
    return timeRequired;
}

bool CBuilding::DoProduce(){
    if(currentProduction == nullptr){
        return false;
    }
    for(auto const &c : currentProduction->GetOutput()){
        AddToInventory(c.first, c.second);
    }
    return true;
}

void CBuilding::Destroy(){
    isBeingDestroyed = true;
    for(auto e : Inhabitants)    {
        if(auto s = e.lock()){
            s->Destroy();
        }
    }
    ApplyMovementCosts(true);
}

bool CBuilding::AddWork(int amount){
    workToComplete -= amount;
    if(workToComplete <= 0){
        workToComplete = 0;
        ApplyMovementCosts();
        for(auto p : CGood::GetResources() ){
            Inventory[p.first] = 0;
        }
        Workers.clear();
        return true;
    }
    return false;
}

int CBuilding::AddToInventory(int resource, int amount){
    if(amount <= GetMaxStorage(resource, true)){
        Inventory.at(resource) += amount;
        return 0;
    }
    int amountAdded = GetMaxStorage(resource, true) - Inventory.at(resource);
    Inventory.at(resource) += amountAdded;
    return amount - amountAdded;
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

void CBuilding::ApplyMovementCosts(bool destroy){
    auto layout = typePtr->GetLayout();
    for(int i = 0; i < GetTileHeight(); i++ ){
        for(int k = 0; k < GetTileWidth(); k++ ){
            if(destroy){
                tile_shared_ptr tile = GAP.ChunkManager.GetTile( tileX + k, tileY + i).lock();
                if(tile){
                    GAP.Pathfinder.SetCost(tileX + k, tileY + i, tile->GetMoveCost() );
                }
                continue;
            }
            if(typePtr->IsRoad() && workToComplete > 0){
                continue;
            }
            GAP.Pathfinder.SetCost(tileX + k, tileY + i, layout.at(i).at(k));
        }
    }
}

int CBuilding::GetMaxStorage(int resource_, bool excludingOrders){
    if(resource_ == CGood::work){
        return 0;
    }
    if(!workToComplete && typePtr->GetStorage()){
        return typePtr->GetStorage();
    }
    int availableStorage = 0;
    if(workToComplete){
        availableStorage = typePtr->BuildCost(resource_);
    }else if(GetResource() == resource_){
        availableStorage = 10;
    }else if(ConsumesResource(resource_)){
        availableStorage = ConsumesResource(resource_) * 5;
    }else if(typePtr->ProducesResource(resource_)){
        availableStorage = typePtr->ProducesResource(resource_) * 5;
    }
    if(availableStorage && !excludingOrders){
        availableStorage -= IncomingByResource(resource_);
    }
    availableStorage -= Inventory.at(resource_);
    if(availableStorage > 0){
        return availableStorage;
    }
    return 0;
}

int CBuilding::GetResourcePrio(int resource_){
    if(typePtr->GetStorage()){
        return 1;
    }
    if(typePtr->ConsumesResource(resource_)){
        return 2;
    }
    if(workToComplete && typePtr->BuildCost(resource_)){
        return 10;
    }
    return -1;
}

build_weak_ptr CBuilding::FindNearestStorage(int resource_){
    build_weak_ptr storage;
    int prioBest = GetResourcePrio(resource_);
    int prioHere = 0;
    int distBest = 999;
    int distHere = 0;
    for(auto w : ConnectedBuildings){
        if(auto s = w.lock()){
            if( s->GetMaxStorage(resource_) ){
                prioHere = s->GetResourcePrio(resource_);
                if( prioHere >= prioBest){
                    if( prioHere > prioBest){
                        distBest = 999;
                        prioBest = prioHere;
                    }
                    distHere = GAP.Pathfinder.FindPath(
                            Coord( DoorX(), DoorY() ),
                            Coord( s->DoorX(), s->DoorY() ),
                            0.0f, 2.0f
                    ).size();
                    if(distHere < distBest){
                        storage = w;
                        distBest = distHere;
                    }
                }
            }
        }
    }
    return storage;
}

int CBuilding::IncomingByResource(int res){
    int amount = 0;
    for(auto w : Incoming)    {
        if(auto s = w.lock()){
            if(s->GetResource() == res){
                amount++;
            }
        }
    }
    return amount;
}

int CBuilding::OutgoingByResource(int res){
    int amount = 0;
    for(auto w : Outgoing)    {
        if(auto s = w.lock()){
            if(s->GetResource() == res){
                amount++;
            }
        }
    }
    return amount;
}

task_weak_ptr CBuilding::FindConnectedTask(unit_weak_ptr worker){
    auto workerSPtr = worker.lock();
    task_weak_ptr bestTask;
    int bestDist = 999, thisDist;
    for(auto wt : GetOutgoing()){
        if(auto st = wt.lock()){
            if(!st->GetPorter().lock()){
                if(auto pickUpS = st->GetPickUp().lock()){
                    thisDist = workerSPtr->GetTileFlightRoundDistance(pickUpS->DoorX(), pickUpS->DoorY());
                    if(thisDist < bestDist){
                        bestDist = thisDist;
                        bestTask = wt;
                    }
                }
            }
        }
    }
    for(auto w : ConnectedBuildings){
        if(auto s = w.lock()){
            for(auto wt : s->GetOutgoing()){
                if(auto st = wt.lock()){
                    if(!st->GetPorter().lock()){
                        if(auto pickUpS = st->GetPickUp().lock()){
                            thisDist = workerSPtr->GetTileFlightRoundDistance(pickUpS->DoorX(), pickUpS->DoorY());
                            if(thisDist < bestDist){
                                bestDist = thisDist;
                                bestTask = wt;
                            }
                        }
                    }
                }
            }
        }
    }
    return bestTask;
}


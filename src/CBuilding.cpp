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
    if(workToComplete && owner >= 0){
        clip.y = clip.h;
    }
    door = typePtr->GetDoor();
    door.first += tileX;
    door.second += tileY;
}

void CBuilding::SetId(int id_, build_weak_ptr ptr){
    id = id_;
    myPtr = ptr;
    if(DistributionRange()){
        name = "Town ";
        std::stringstream tmp;
        tmp << id;
        name.append(tmp.str());
    }
    std::vector<task_weak_ptr> taskVec;
    maxAssignedWorkers = MaxWorkers();
    for(auto & recipe : *(typePtr->GetRecipes()) ){
        CRecipe recipeCopy = *recipe;
        recipeCopy.SetParent(recipe);
        Recipes.push_back(recipeCopy);
    }
    for(auto p : CGood::GetResources() ){
        Inventory[p.first] = 0;
        if(DistributionRange()){
            RequestedGoods[p.first] = taskVec;
            OfferedGoods[p.first] = taskVec;
        }
    }
    ConnectToNearestTown();
    UpdateNeededGoods();

}

bool CBuilding::Render(){
    if(workToComplete && owner >= 0){
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

bool CBuilding::RenderInPosition(GPU_Rect* destBox){
    GAP.TextureManager.DrawTextureGL(typePtr->GetName(), &clip, destBox, true);
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
    if(auto s = GAP.BuildingManager.GetBuildingAt(tileX, tileY).lock()){
        if(s->GetTileX() == GetTileX() && s->GetTileY() == GetTileY() && !GetName()->compare(*s->GetUpgrade())){
            return false;
        }
    }
    if(DistributionRange()){
        for(auto const & tw : *(GAP.BuildingManager.GetTowns())){
            if(auto ts = tw.lock()){
                if(ts->GetTileFlightRoundDistance(tileX, tileY) < DistributionRange()){
                    return true;
                }
            }
        }
    }
    for(int i = 0; i < GetTileHeight(); i++ ){
        for(int k = 0; k < GetTileWidth(); k++ ){
            if(GAP.Pathfinder.GetCost(tileX + k, tileY + i) != CScreen::flatMoveCost ){
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

void CBuilding::Destroy(){
    isBeingDestroyed = true;
    for(auto e : Inhabitants)    {
        if(auto s = e.lock()){
            s->Destroy();
        }
    }
    for(auto e : Traders)    {
        if(auto s = e.lock()){
            s->Destroy();
        }
    }
    for(auto e : Incoming)    {
        if(auto s = e.lock()){
            s->MarkComplete();
        }
    }
    for(auto e : Outgoing)    {
        if(auto s = e.lock()){
            s->MarkComplete();
        }
    }
    if(auto s = myTown.lock()){
        s->RemoveConnection(id);
    }
    ApplyMovementCosts(true);
}

bool CBuilding::AddWork(int amount, bool setUp){
    workToComplete -= (amount * GAP.Setting(CSettingManager::BuildSpeed) ); // 
    if(workToComplete < 1){
        workToComplete = 0;
        clip.y = 0;
        ApplyMovementCosts();
        for(auto p : CGood::GetResources() ){
            Inventory[p.first] = 0;
        }
        Workers.clear();
        if(DistributionRange()){
            ConnectToNearestTown();
        }
        UpdateNeededGoods();
        return true;
    }
    return false;
}

///////

void CBuilding::Update(){
    if(!IsRoad() && !DistributionRange() && GetConnections()->size() < 1){
        ConnectToNearestTown(true);
    }
    if(!workToComplete){
        if(currentProductionCooldown > 0){
            currentProductionCooldown--;
        }
        DeleteUnusedWeak(&Workers);
        DeleteUnusedWeak(&Traders);
        if(MaxPop() > static_cast<int>(Inhabitants.size())){
            popProgress++;
            if(popProgress >= typePtr->PopCost()){
                popProgress = 0;
                unit_shared_ptr is = std::make_shared<CUnit>(door.first, door.second, *typePtr->GetInhabitant());
                unit_weak_ptr iw = unit_weak_ptr(is);

                GAP.UnitManager.AddNPC(is);
                is->SetHome(myPtr);
                is->SetSkill(InhabitantSkill());
                is->SetIdleAssignment();
                Inhabitants.push_back(iw);
            }
        }else if(MaxTraders() > static_cast<int>(Traders.size())){
            popProgress++;
            if(popProgress >= typePtr->PopCost()){
                popProgress = 0;
                unit_shared_ptr is = std::make_shared<CUnit>(door.first, door.second, "trader");
                unit_weak_ptr iw = unit_weak_ptr(is);

                GAP.UnitManager.AddNPC(is);
                is->SetHome(myPtr);
                is->SetTraderAssignment();
                Traders.push_back(iw);
            }
        }
    }
    UpdateTransportTasks();
}

void CBuilding::MatchTransportTasks(){
    if(workToComplete){
        return;
    }
    for(auto goodsVec : OfferedGoods){
        DeleteUnusedWeak(&goodsVec.second);
        DeleteUnusedWeak(&RequestedGoods.at(goodsVec.first));
        for(auto ow : goodsVec.second){
            if(auto os = ow.lock()){
                if(!os->GetCompleted()){
                    int bestPrio = 0;
                    task_weak_ptr bestDest;
                    for(auto dw :  RequestedGoods.at(goodsVec.first) ){
                        if(auto ds = dw.lock()){
                            if(ds->GetCompleted()){
                                continue;
                            }
                            if(ds->GetPrio() <= bestPrio){
                                continue;
                            }
                            bestPrio = ds->GetPrio();
                            bestDest = dw;
                        }
                    }
                    if(auto bs = bestDest.lock()){
                        if(auto destS = bs->GetDropOff().lock()){
                            if(auto oriS = os->GetPickUp().lock()){
                                task_shared_ptr ts = std::make_shared<CTransportTask>(
                                                          os->GetPickUp(),
                                                          bs->GetDropOff(),
                                                          goodsVec.first,
                                                          bestPrio);
                                if(ts->CheckPath()){
                                    task_weak_ptr tw = task_weak_ptr(ts);

                                    destS->AddIncoming(tw);
                                    oriS->AddOutgoing(tw);

                                    AvailableTasks.push_back(tw);
                                    GAP.TaskManager.AddTask(ts);

                                    bs->MarkComplete();
                                    os->MarkComplete();
                                }
                            }
                        }
                    }else if(!os->OfferedByTown()){
                        // if something is being sent from a building other than the town and has no place to go
                        // take it into the town
                        if(auto oriS = os->GetPickUp().lock()){
                            task_shared_ptr ts = std::make_shared<CTransportTask>(
                                                      os->GetPickUp(),
                                                      myPtr,
                                                      goodsVec.first,
                                                      bestPrio);
                            if(ts->CheckPath()){
                                task_weak_ptr tw = task_weak_ptr(ts);

                                AddIncoming(tw);
                                oriS->AddOutgoing(tw);

                                AvailableTasks.push_back(tw);
                                GAP.TaskManager.AddTask(ts);

                                os->MarkComplete();
                            }
                        }
                    }
                }
            }
        }
    }
}

void CBuilding::UpdateWorkPositions(){
    ConstructionJobs.clear();
    NoblemanJobs.clear();
    CitizenJobs.clear();
    PeasantJobs.clear();
    BruteJobs.clear();
    for(auto wc : *(GetConnections())){
        if(auto sc = wc.lock()){
            if(!sc->HasEnoughWorkers()){
                if(sc->UnderConstruction() && sc->HasBuildingResources()){
                    ConstructionJobs.push_back(wc);
                    continue;
                }
                if(sc->HasWorkToDo(3)){
                    NoblemanJobs.push_back(wc);
                }
                if(sc->HasWorkToDo(2)){
                    CitizenJobs.push_back(wc);
                }
                if(sc->HasWorkToDo(1)){
                    PeasantJobs.push_back(wc);
                }
                if(sc->HasWorkToDo(0)){
                    BruteJobs.push_back(wc);
                }
            }
        }
    }
}

build_weak_ptr CBuilding::GetWorkPlace(unit_weak_ptr worker){
    if(DistributionRange() == 0){
        if(auto s = myTown.lock()){
            return s->GetWorkPlace(worker);
        }
        return build_weak_ptr();
    }
    build_weak_ptr job;
    if(auto s = worker.lock()){
        if(s->GetSkill() == 3){
            if(NoblemanJobs.size() > 0){
                job = NoblemanJobs.back();
                NoblemanJobs.pop_back();
            }else if(CitizenJobs.size() > 0){
                job = CitizenJobs.back();
                CitizenJobs.pop_back();
            }
        }
        if(s->GetSkill() == 2){
            if(CitizenJobs.size() > 0){
                job = CitizenJobs.back();
                CitizenJobs.pop_back();
            }else if(PeasantJobs.size() > 0){
                job = PeasantJobs.back();
                PeasantJobs.pop_back();
            }
        }
        if(s->GetSkill() == 1){
            if(PeasantJobs.size() > 0){
                job = PeasantJobs.back();
                PeasantJobs.pop_back();
            }else if(BruteJobs.size() > 0){
                job = BruteJobs.back();
                BruteJobs.pop_back();
            }
        }
        if(s->GetSkill() == 0){
            if(BruteJobs.size() > 0){
                job = BruteJobs.back();
                BruteJobs.pop_back();
            }
        }
    }
    if(auto jobS = job.lock()){
        if(jobS->HasEnoughWorkers()){
            return build_weak_ptr();
        }
    }
    return job;
}

build_weak_ptr CBuilding::GetBuildPlace(unit_weak_ptr worker){
    if(DistributionRange() == 0){
        if(auto s = myTown.lock()){
            return s->GetBuildPlace(worker);
        }
        return build_weak_ptr();
    }
    build_weak_ptr job;
    if(auto s = worker.lock()){
        if(s->GetSkill() < 3){
            while(ConstructionJobs.size() > 0){
                job = ConstructionJobs.back();
                ConstructionJobs.pop_back();
                if(auto sc = job.lock()){
                    if(!sc->HasEnoughWorkers() && sc->UnderConstruction() && sc->HasBuildingResources()){
                        return job;
                    }
                }
            }
        }
    }
    return build_weak_ptr();
}

task_weak_ptr CBuilding::FindPlaceToDropOff(int resource){
    if(!DistributionRange()){
        if(auto s = myTown.lock()){
            return s->FindPlaceToDropOff(resource);
        }
    }
    int bestPrio = 0;
    task_weak_ptr bestDest;
    task_weak_ptr tw;
    for(auto dw :  RequestedGoods.at(resource) ){
        if(auto ds = dw.lock()){
            if(!ds->GetCompleted() && ds->GetPrio() > bestPrio){
                bestPrio = ds->GetPrio();
                bestDest = dw;
            }
        }
    }
    if(auto bs = bestDest.lock()){
        if(auto destS = bs->GetDropOff().lock()){
            task_shared_ptr ts = std::make_shared<CTransportTask>(
                                      build_weak_ptr(),
                                      bs->GetDropOff(),
                                      resource,
                                      bestPrio);
            tw = task_weak_ptr(ts);

            destS->AddIncoming(tw);
            GAP.TaskManager.AddTask(ts);
            return tw;
        }
    }else{
        task_shared_ptr ts = std::make_shared<CTransportTask>(
                                  build_weak_ptr(),
                                  myPtr,
                                  resource,
                                  bestPrio);
        tw = task_weak_ptr(ts);

        AddIncoming(tw);
        GAP.TaskManager.AddTask(ts);
    }
    return tw;
}

void CBuilding::UpdateTransportTasks(){
    DeleteUnusedWeak(&Incoming);
    DeleteUnusedWeak(&Outgoing);
    if(auto ts = myTown.lock()){
        for (auto const& item : Inventory){
            if(item.first == CGood::work){
                continue;
            }
            if(NeededGoods.count(item.first)){
                if(item.second < NeededGoods.at(item.first)){
                    if(!IncomingByResource(item.first, true)){
                        if(item.second + IncomingByResource(item.first) < NeededGoods.at(item.first)){
                            ts->AddResourceRequest(myPtr, item.first, resourcePrio);
                        }
                    }
                }
            }else if(item.second > 0){
                if(!OutgoingByResource(item.first, true)){
                    if(item.second > OutgoingByResource(item.first)){
                        ts->AddResourceOffer(myPtr, item.first);
                    }
                }
            }
        }
    }
}

void CBuilding::UpdateNeededGoods(){
    NeededGoods.clear();
    if(UnderConstruction()){
        for(auto const &c : typePtr->GetBuildCosts()){
            if( c.first != CGood::work){
                NeededGoods[c.first] = c.second;
            }
        }
    }else{
        for(auto p : CGood::GetResources() ){
            if(ConsumesResource(p.first)){
                NeededGoods[p.first] = ConsumesResource(p.first) * 5;
            }
        }
    }
}

////////////////

void CBuilding::AddIncoming(task_weak_ptr ptr){
    Incoming.push_back(ptr);
}

void CBuilding::RemoveIncoming(int id){
    if(!isBeingDestroyed){
        DeleteById(&Incoming, id);
    }
}

void CBuilding::AddOutgoing(task_weak_ptr ptr){
    Outgoing.push_back(ptr);
}

void CBuilding::RemoveOutgoing(int id){
    if(!isBeingDestroyed){
        DeleteById(&Outgoing, id);
    }
}

void CBuilding::AddWorker(unit_weak_ptr ptr){
    Workers.push_back(ptr);
}

void CBuilding::RemoveWorker(int id){
    if(!isBeingDestroyed){
        DeleteById(&Workers, id);
    }
}

void CBuilding::AddConnection(build_weak_ptr ptr){
    if(DistributionRange()){
        if(auto s = ptr.lock()){
            RemoveConnection(s->GetId());
            if(s->DistributionRange() && s->GetId() != id && !s->UnderConstruction()){
                ConnectedTowns.push_back(ptr);
            }else{
                ConnectedBuildings.push_back(ptr);
            }
        }
    }
}

void CBuilding::RemoveConnection(int id_){
    if(!isBeingDestroyed){
        DeleteById(&ConnectedBuildings, id_);
        DeleteById(&ConnectedTowns, id_);
    }
}

void CBuilding::ClearConnections(){
    ConnectedBuildings.clear();
    myTown.reset();
}

void CBuilding::RemoveInhabitant(int id){
    if(!isBeingDestroyed){
        DeleteById(&Inhabitants, id);
    }
}

////////////////

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
    return static_cast<int>(Workers.size()) >= MaxAssignedWorkers() ;
}

int CBuilding::MaxAssignedWorkers(int newMax){
    if(workToComplete){
        return 1;
    }
    if(newMax == -1){
        return maxAssignedWorkers;
    }else if(newMax < 1){
        maxAssignedWorkers = 0;
    }else if(newMax > MaxWorkers()){
        maxAssignedWorkers = MaxWorkers();
    }else{
        maxAssignedWorkers = newMax;
    }
    int previousSize = Workers.size() + 1;
    while(maxAssignedWorkers < static_cast<int>(Workers.size()) && static_cast<int>(Workers.size()) != previousSize){
        previousSize = Workers.size();
        DeleteUnusedWeak(&Workers);
        if(auto s = Workers.back().lock()){
            s->SetIdleAssignment();
        }
    }
    return maxAssignedWorkers;
}

bool CBuilding::HasWorkToDo(int skill){
    if(workToComplete){
        return false;
    }
    if(MaxAssignedWorkers() < 0){
        return false;
    }
    if(skill < 1 && ResourceArea() > 0
       && GAP.ChunkManager.GetResources(GetResource(), ResourceArea(), DoorX(), DoorY()).size() > 0){
        return true;
    }
    if(CanProduce(skill)){
        return true;
    }
    return false;
}

bool CBuilding::CanProduce(int skill){
    if(MaxAssignedWorkers() < 0){
        return false;
    }
    if(currentProductionCooldown > 0){
        return false;
    }
    if(AvailableRecipe(skill) == nullptr){
        return false;
    }
    return true;
}

CRecipe* CBuilding::AvailableRecipe(int skill){
    if(!ConsumesResource(0)){
        return nullptr;
    }
    int bestRecipePrio = 0;
    CRecipe* bestRecipe = nullptr;
    for(auto & recipe : Recipes ){
        if(!recipe.IsAvailable()){
            continue;
        }
        if(recipe.GetSkill() > skill || recipe.GetSkill() < (skill - 1)){
            continue;
        }
        bool canProduce = true;
        for(auto const & c : *(recipe.GetInput()) ){
            if( c.first != CGood::work && Inventory.at(c.first) < c.second){
                canProduce = false;
            }
        }
        if(!canProduce){
            continue;
        }
        for(auto const &c : *(recipe.GetOutput()) ){
            if( c.second > GetMaxStorage(c.first) ){
                canProduce = false;
            }
            if(c.first == CGood::bruteResearch){
                if(!GAP.TechManager.IsResearching() || GAP.TechManager.CurrentTechLvL() != 0){
                    canProduce = false;
                }
            }
            if(c.first == CGood::peasantResearch){
                if(!GAP.TechManager.IsResearching() || GAP.TechManager.CurrentTechLvL() != 1){
                    canProduce = false;
                }
            }
            if(c.first == CGood::citizenResearch){
                if(!GAP.TechManager.IsResearching() || GAP.TechManager.CurrentTechLvL() != 2){
                    canProduce = false;
                }
            }
            if(c.first == CGood::noblemenResearch){
                if(!GAP.TechManager.IsResearching() || GAP.TechManager.CurrentTechLvL() != 3){
                    canProduce = false;
                }
            }
        }
        if(!canProduce){
            continue;
        }
        int currentPrio = recipe.NextProductionProgress();
        if(currentPrio > bestRecipePrio){
            bestRecipePrio = currentPrio;
            bestRecipe = &recipe;
        }
    }
    return bestRecipe;
}

int CBuilding::StartProduction(int skill){
    int timeRequired = 0;
    currentProduction = AvailableRecipe(skill);
    if(currentProduction == nullptr){
        return timeRequired;
    }
    for(auto const &c : *(currentProduction->GetInput()) ){
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

    currentProductionCooldown = typePtr->ProductionCooldown();
    if(typePtr->ProductionStages()){
        currentProductionStage++;
        clip.x = currentProductionStage * GetW();

        if(currentProductionStage < typePtr->ProductionStages()){
            return true;
        }

        currentProductionStage -= typePtr->ProductionSetback();

        clip.x = currentProductionStage * GetW();
    }


    for(auto const &c : *(currentProduction->GetOutput()) ){
        AddToInventory(c.first, c.second);
    }
    currentProduction->FinishProduction();
    return true;
}

int CBuilding::AddToInventory(int resource, int amount){
    if(resource == CGood::bruteResearch ||
       resource == CGood::peasantResearch ||
       resource == CGood::citizenResearch ||
       resource == CGood::noblemenResearch){
        GAP.TechManager.AddProgress(resource);
        return 0;
    }
    if(amount <= GetMaxStorage(resource, true)){
        Inventory.at(resource) += amount;
        if(workToComplete && HasBuildingResources()){
            if(auto s = myTown.lock()){
                s->UpdateWorkPositions();
            }
        }
        return 0;
    }
    int amountAdded = GetMaxStorage(resource, true) - Inventory.at(resource);
    Inventory.at(resource) += amountAdded;
    if(workToComplete && HasBuildingResources()){
        if(auto s = myTown.lock()){
            s->UpdateWorkPositions();
        }
    }
    return amount - amountAdded;
}

bool CBuilding::InventoryAvailable(){
    return (GAP.GetTick() - lastItemPickedUp > static_cast<unsigned int>(GAP.Setting(CSettingManager::BuildingInventoryUseCooldown)));
}

void CBuilding::UseInventory(){
    lastItemPickedUp = GAP.GetTick();
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
                GAP.Pathfinder.SetCost(tileX + k, tileY + i, CScreen::flatMoveCost );
                continue;
            }
            if(typePtr->IsRoad() && workToComplete > 0){
                continue;
            }
            if(layout.at(i).at(k) < 3.0f){
                vec2i passableTile;
                passableTile.first = tileX + k;
                passableTile.second = tileY + i;
                PassableTiles.push_back(passableTile);
            }
            GAP.Pathfinder.SetCost(tileX + k, tileY + i, layout.at(i).at(k));
        }
    }
}

vec2i CBuilding::GetRandomPassableTile(){
    if(PassableTiles.size() < 2){
        return PassableTiles.at(0);
    }
    return PassableTiles.at(rand() % PassableTiles.size());
}

bool CBuilding::IsValidWorkLocation(int x, int y){
    if(x < tileX){
        return false;
    }
    if(y < tileY){
        return false;
    }
    if(x >= tileX + GetTileWidth()){
        return false;
    }
    if(y >= tileY + GetTileHeight()){
        return false;
    }
    return true;
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
        availableStorage = GAP.Setting(CSettingManager::GathererBuildingStorage);
    }else if(ConsumesResource(resource_)){
        availableStorage = ConsumesResource(resource_) * GAP.Setting(CSettingManager::ProductionBuildingInputStorage);
    }else if(typePtr->ProducesResource(resource_)){
        availableStorage = typePtr->ProducesResource(resource_) * GAP.Setting(CSettingManager::ProductionBuildingOutputStorage);
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

int CBuilding::IncomingByResource(int res, bool onlyPending){
    int amount = 0;
    for(auto w : Incoming)    {
        if(auto s = w.lock()){
            if(res == s->GetResource()){
                if(onlyPending){
                    if(auto ds = s->GetPickUp().lock()){
                    }else{
                        amount++;
                    }
                }else{
                    amount++;
                }
            }
        }
    }
    return amount;
}

int CBuilding::OutgoingByResource(int res, bool onlyPending){
    int amount = 0;
    for(auto w : Outgoing)    {
        if(auto s = w.lock()){
            if(s->GetResource() == res){
                if(onlyPending){
                    if(auto ds = s->GetDropOff().lock()){
                    }else{
                        amount++;
                    }
                }else{
                    amount++;
                }
            }
        }
    }
    return amount;
}

task_weak_ptr CBuilding::FindConnectedTask(int x, int y){
    task_weak_ptr task;
    bool firstTask = true;
    if(DistributionRange()){
        if(AvailableTasks.size()){
            for(auto tw : AvailableTasks){
                if(auto ts = tw.lock()){
                    if(!ts->GetAssigned()){
                        if(firstTask){
                            task = tw;
                        }
                        if(ts->GetDoor().first == x && ts->GetDoor().second == y){
                            return tw;
                        }
                    }
                }
            }
        }
        return task;
    }
    if(auto s = myTown.lock()){
        return s->FindConnectedTask(x, y);
    }
    return task;
}

std::vector<build_weak_ptr>* CBuilding::GetConnections(){
    if(DistributionRange() == 0){
        if(auto s = myTown.lock()){
            return s->GetConnections();
        }
    }
    return &ConnectedBuildings;
}

std::vector<build_weak_ptr>* CBuilding::GetConnectedTowns(){
    return &ConnectedTowns;
}

void CBuilding::ConnectToNearestTown(bool forceReconnect){
    if(DistributionRange() > 0){
        if(!workToComplete){
            if(auto s = myTown.lock()){
                s->RemoveConnection(id);
            }
            ConnectedBuildings.clear();
            myTown = myPtr;
            ConnectedBuildings.push_back(build_weak_ptr(myPtr));
            for(auto cw : ConnectedTowns){
                if(auto cs = cw.lock()){
                    cs->RemoveConnection(id);
                }
            }
            ConnectedTowns.clear();
            for(auto const & tw : *(GAP.BuildingManager.GetTowns())){
                if(auto ts = tw.lock()){
                    if(ts->GetTileFlightRoundDistance(tileX, tileY) < ts->DistributionRange() * GAP.Setting(CSettingManager::MaxTownConnectionRangeFactor)){
                        AddConnection(tw);
                        ts->AddConnection(myPtr);
                    }
                }
            }
        return;
        }
    }
    if(IsRoad()){
        return;
    }
    int closestDist = 9999999;
    if(auto s = myTown.lock()){
        if(!forceReconnect){
            return;
        }else{
            closestDist = s->GetTileFlightRoundDistance(tileX, tileY);
            s->RemoveConnection(id);
        }
    }
    for(auto const & tw : *(GAP.BuildingManager.GetTowns())){
        if(auto ts = tw.lock()){
            if( !ts->UnderConstruction()
                && ts->GetTileFlightRoundDistance(tileX, tileY) < closestDist
                && (ts->GetTileFlightRoundDistance(tileX, tileY) < ts->DistributionRange()
                    || (DistributionRange() && workToComplete))){

                closestDist = ts->GetTileFlightRoundDistance(tileX, tileY);
                myTown = tw;
            }
        }
    }
    if(auto s = myTown.lock()){
        s->AddConnection(myPtr);
    }
}

void CBuilding::AddResourceOffer(build_weak_ptr sourcePtr, int res){
    if( DistributionRange() ){
        if(auto ss = sourcePtr.lock()){
            task_shared_ptr ts = std::make_shared<CTransportTask>(sourcePtr, build_weak_ptr(), res, 0);
            if(ss->GetType() == type){
                // the town itself is sending out this offer
                ts->SetOfferedByTown();
            }
            task_weak_ptr tw = task_weak_ptr(ts);
            GAP.TaskManager.AddTask(ts);
            ss->AddOutgoing(tw);
            OfferedGoods.at(res).push_back(tw);
        }
    }
}

void CBuilding::AddResourceRequest(build_weak_ptr destPtr, int res, int prio){
    if( DistributionRange() ){
        if(auto ss = destPtr.lock()){
            task_shared_ptr ts = std::make_shared<CTransportTask>(build_weak_ptr(), destPtr, res, prio);
            task_weak_ptr tw = task_weak_ptr(ts);
            GAP.TaskManager.AddTask(ts);
            ss->AddIncoming(tw);
            RequestedGoods.at(res).push_back(tw);
        }
    }
}

build_weak_ptr CBuilding::GetNextTraderDestination(){
    bool foundCurrent = false;
    DeleteUnusedWeak(&ConnectedTowns);
    for(auto t : ConnectedTowns){
        if(foundCurrent){
            return t;
        }
        if(auto s = t.lock()){
            if(s->GetId() == lastVisitedTownId){
                foundCurrent = true;
            }
        }
    }
    if(ConnectedTowns.size()){
        return ConnectedTowns.back();
    }
    return build_weak_ptr();
}


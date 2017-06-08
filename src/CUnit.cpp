#include "CUnit.h"
#include "CGame.h"

extern CGame GAP;
typedef std::weak_ptr<CUnit> unit_weak_ptr;

CUnit::CUnit(int x_, int y_, std::string myName){
    tileX = x_;
    x = x_ * CScreen::tileWidth ;
    tileY = y_;
    y = y_ * CScreen::tileWidth ;
    xoff = - ((CScreen::unitWidth - CScreen::tileWidth) / 2);
    yoff = - (CScreen::unitWidth / 2);
    name = myName;
    box.h = CScreen::unitWidth;
    box.w = CScreen::unitWidth;
    itemBox.h = CScreen::tileWidth;
    itemBox.w = CScreen::tileWidth;
    assignment = CAction::idleAssignment;
    for(auto p : CGood::GetResources() ){
        Inventory[p.first] = 0;
    }
    this->ResetFrames();
}

bool CUnit::PreRender(){

    box.x = x + xoff;
    box.y = y + yoff;
    if(currentFrameStarted + currentFrameSpeed < GAP.GetTick() ){
        currentFrameStarted = GAP.GetTick();
        currentFrame++;
        if(currentFrame >= frameCount){
            currentFrame = 0;
        }
    }
    return 1;
}

bool CUnit::Render(){

    PreRender();

    GAP.TextureManager.DrawTextureGL(&name, GAP.TextureManager.GetClip(currentFrame + frameOffset), &box);
    if(carriedItem){
        itemBox.x = x;
        itemBox.y = y;
        CGood::Render(&itemBox, carriedItem);
    }
    return 1;
}

bool CUnit::RenderInPosition(GPU_Rect* destBox){
    GAP.TextureManager.DrawTextureGL(&name, GAP.TextureManager.GetClip(currentFrame + frameOffset), destBox, true);
    return 1;
}

void CUnit::Update(){

    if(!busyTime){
        if(!ActionQueue.size()){
            UpdateAssignment();
        }
        if(ActionQueue.size()){
            isIdle = false;
            currentAction = ActionQueue.front();
            ActionQueue.erase (ActionQueue.begin());
            busyTime = ExecuteAction(currentAction);
        }else{
            StopMovement();
        }
    }else if(busyTime == 1){
        FinishAction(currentAction);
    }
    if(moving){
        float pixelsPerTick = (float)CScreen::tileWidth / moveSpeed;
        int pixelsMovedNow = (pixelsPerTick + carryOverPixels);
        carryOverPixels = (pixelsPerTick + carryOverPixels) - pixelsMovedNow;
        switch(moveDirection){
            case 0:
                this->y -= pixelsMovedNow;
                break;
            case 1:
                this->y -= pixelsMovedNow;
                this->x -= pixelsMovedNow;
                break;
            case 2:
                this->x -= pixelsMovedNow;
                break;
            case 3:
                this->y += pixelsMovedNow;
                this->x -= pixelsMovedNow;
                break;
            case 4:
                this->y += pixelsMovedNow;
                break;
            case 5:
                this->y += pixelsMovedNow;
                this->x += pixelsMovedNow;
                break;
            case 6:
                this->x += pixelsMovedNow;
                break;
            case 7:
                this->y -= pixelsMovedNow;
                this->x += pixelsMovedNow;
                break;
        }
    }
    if(busyTime > 0){
        busyTime--;
    }
}

std::string CUnit::GetAssignmentName() const{
    switch(assignment){
        case CAction::idleAssignment: return "Idle";
        case CAction::followUnit: return "Following a unit";
        case CAction::builderAssignment: return "Building";
        case CAction::gatherResources: return "Gathering resources";
    }
    return "No assignment";
}

void CUnit::ResetFrames(){
    SetFrameCount();
    SetFrameOffset();
    SetFrameSpeed();
    x = GAP.TileToPixel(tileX);
    y = GAP.TileToPixel(tileY);
    currentFrame = 0;
    currentFrameStarted = GAP.GetTick();
}

void CUnit::SetFacing(int direction){
    switch(direction){
        case 0:
            facing = 0; break;
        case 1:
        case 2:
        case 3:
            facing = 1; break;
        case 4:
            facing = 2; break;
        case 5:
        case 6:
        case 7:
            facing = 3; break;
    }
    if(direction != moveDirection){
        moveDirection = direction;
        this->ResetFrames();
    }
    moveDirection = direction;
}

void CUnit::SetMoving(bool isMoving){
    if(isMoving){
        this->SetAnimation("walking");
    }
    else{
        this->SetAnimation("standing");
    }
    moving = isMoving;
}

void CUnit::SetAnimation(std::string animation){
    int newAnimation;
    if(animation.compare("casting") == 0){
        newAnimation = 1;
    }
    if(animation.compare("thrusting") == 0 ){
        newAnimation = 2;
    }
    if(animation.compare("walking") == 0){
        newAnimation = 3;
    }
    if(animation.compare("slashing") == 0 ){
        newAnimation = 4;
    }
    if(animation.compare("shooting") == 0 ){
        newAnimation = 5;
    }
    if(animation.compare("hurting") == 0 ){
        newAnimation = 6;
    }
    if(animation.compare("standing") == 0 ){
        newAnimation = 7;
    }
    if(currentAnimation != newAnimation){
        currentAnimation = newAnimation;
        this->ResetFrames();
    }
}

int CUnit::SetFrameCount(){
    switch(currentAnimation){
        case 1:
            frameCount = 7;
            return 7;
        case 2:
            frameCount = 8;
            return 8;
        case 3:
            frameCount = 9;
            return 9;
        case 4:
            frameCount = 6;
            return 6;
        case 5:
            frameCount = 13;
            return 13;
        case 6:
            frameCount = 6;
            return 6;
        case 7:
            frameCount = 1;
            return 1;
        default:
            frameCount = 6;
            return 6;
    }
}

int CUnit::SetFrameSpeed(){
    switch(currentAnimation){
        case 3:
            currentFrameSpeed = 4;
            break;
        default:
            currentFrameSpeed = 5;
            break;
    }
    return currentFrameSpeed;
}

int CUnit::SetFrameOffset(){
    if(currentAnimation == 6){
        this->frameOffset = this->BaseOffset();
    }else{
        this->frameOffset = this->BaseOffset() + (13 * facing);
    }
    return this->frameOffset;
}

int CUnit::BaseOffset(){
    switch(currentAnimation){
        case 1:
            return 0;
        case 2:
            return 52;
        case 3:
        case 7:
            return 104;
        case 4:
            return 156;
        case 5:
            return 208;
        case 6:
            return 260;
        default:
            return 0;
    }
}

void CUnit::AddAction(CAction action_){
    ActionQueue.push_back(action_);
    if(isIdle){
        busyTime = 0;
    }
}

int CUnit::ExecuteAction(CAction action_){
    int timeForAction = 0;
    switch(action_.type){
        case CAction::idleAction:
            timeForAction = action_.arga;
            StopMovement();
            isIdle = true;
            break;
        case CAction::moveTile:
            if(MoveToAdyacent(action_.arga, action_.argb)){
                timeForAction = moveSpeed;
            }else{
                ClearActions();
                StopMovement();
            }
            break;
        case CAction::gatherResource:
            if(auto s = resourceTilePtr.lock()){
                if(s->GetResource()){
                    StopMovement();
                    SetAnimation("slashing");
                    SetFacing(GetTileDirection(s->GetTileX(), s->GetTileY()));
                    timeForAction = action_.arga;
                }
            }
            break;
        case CAction::dropGoods:
            StopMovement();
            Inventory.clear();
            timeForAction = action_.arga;
            break;
        case CAction::buildBuilding:
            StopMovement();
            SetAnimation("slashing");
            timeForAction = action_.arga;
            break;
        case CAction::doProduce:
            StopMovement();
            if(auto s = workBuildingPtr.lock()){
                SetAnimation("slashing");
                timeForAction = s->StartProduction(skillLvl);
                if(!timeForAction){
                    SetIdleAssignment();
                }
            }
            break;
    }
    return timeForAction;
}

int CUnit::FinishAction(CAction action_){
    switch(action_.type){
        case CAction::idleAction:
            break;
        case CAction::moveTile:
            break;
        case CAction::gatherResource:
            if(auto s = resourceTilePtr.lock()){
                if(s->HarvestResource()){
                    CarryItem(action_.argb);
                    if(!s->GetResource()){
                        resourceTilePtr.reset();
                    }
                }
            }
            break;
        case CAction::buildBuilding:
            if(auto s = targetBuildingPtr.lock()){
                if(s->AddWork(action_.argb)){
                    targetBuildingPtr.reset();
                }
            }
            break;
        case CAction::doProduce:
            if(auto s = workBuildingPtr.lock()){
                s->DoProduce();
            }
            break;
    }
    return 0;
}

void CUnit::StopMovement(){
    ResetFrames();
    isIdle = true;
    SetMoving(false);
}

bool CUnit::MoveToAdyacent(int x_, int y_){
    if(GetTileFlightSquareDistance(x_, y_) > 1 || (x == x_  && y == y_)){
        return false;
    }
//    float moveCost = GAP.Pathfinder.GetCost(x_, y_);
    if(GAP.Pathfinder.GetCost(x_, y_) >= GetMaxCollision()){
        return false;
    }
    moveSpeed = baseSpeed;
//    if(moveCost < 2.0f){
//        moveSpeed = 14;
//    }
//    if(moveCost < 1.0f){
//        moveSpeed = 10;
//    }
    SetMoving(true);
    SetFacing(GetTileDirection(x_, y_));
    GAP.ChunkManager.MoveUnit(tileX, tileY, x_, y_, myPtr, id);
    tileX = x_;
    tileY = y_;
    carryOverPixels = 0.0f;
    return true;
}

float CUnit::GetMoveSpeed(){
    return ( (float)CScreen::tileWidth / (float)moveSpeed ) ;
}

void CUnit::ClearActions(){
    ActionQueue.clear();
}

bool CUnit::MoveTo(int x, int y, int stopShort){
    ClearActions();
    CoordList path = GAP.Pathfinder.FindPath(
        Coord( GetTileX(), GetTileY() ),
        Coord( x, y ),
        GetMinCollision(), GetMaxCollision()
    );
    if(path.size() < 1){
        return false;
    }
    for(Coord& c : path){
        CAction action = CAction(CAction::moveTile, c.x, c.y);
        AddAction(action);
    }
    for(int i = 0; i < stopShort; i++){
        ActionQueue.pop_back();
    }
    return true;
}

bool CUnit::MoveNextTo(int x, int y, bool diagonal){
    ClearActions();
    CoordList adyacentTiles = GAP.Pathfinder.getAdyacentTiles(x,y,diagonal);
    std::random_shuffle ( adyacentTiles.begin(), adyacentTiles.end() );
    CoordList path;
    while(path.size() < 1 && adyacentTiles.size() > 0){
        path = GAP.Pathfinder.FindPath(
            Coord( GetTileX(), GetTileY() ),
            Coord( adyacentTiles.back().x, adyacentTiles.back().y ),
            GetMinCollision(), GetMaxCollision());
        adyacentTiles.pop_back();
    }
    if(path.size() < 1){
        return false;
    }
    for(Coord& c : path){
        CAction action = CAction(CAction::moveTile, c.x, c.y);
        AddAction(action);
    }
    return true;
}

void CUnit::Idle(int time, bool addOnly){
    if(!addOnly){
        ClearActions();
    }
    CAction action = CAction(CAction::idleAction, time, 0);
    AddAction(action);
}

void CUnit::SetIdleAssignment(){
    ClearActions();
    assignment = CAction::idleAssignment;
    maxCollision = 3.0f;
    if(auto s = workBuildingPtr.lock()){
        s->RemoveWorker(id);
    }
    if(auto s = targetBuildingPtr.lock()){
        s->RemoveWorker(id);
    }
    workBuildingPtr.reset();
    UpdateAssignment();
}

void CUnit::SetTraderAssignment(){
    ClearActions();
    assignment = CAction::traderAssignment;
    maxCollision = 2.0f;
    baseSpeed = 32;
    UpdateAssignment();
}

void CUnit::SetFollowAssignment(unit_weak_ptr targetPtr_){
    ClearActions();
    if(targetPtr_.lock()){
        targetUnitPtr = targetPtr_;
        assignment = CAction::followUnit;
    }
    maxCollision = 3.0f;
    UpdateAssignment();
}

bool CUnit::SetTransportAssignment(task_weak_ptr ptr){
    if(auto taskPtrS = taskPtr.lock()){
        if(GAP.Pathfinder.GetCost(tileX, tileY) <= 2.0f){
            maxCollision = 2.0f;
        }
        ClearActions();
        taskPtrS->AssignPorter(myPtr);
        thought =  "Found a transport task";
        assignment = CAction::transportAssignment;
        return true;
    }
    return false;
}

void CUnit::UpdateIdleAssignment(){
    if(auto s = homeBuildingPtr.lock()){
        if(GAP.Pathfinder.GetCost(tileX, tileY ) >= 2.0f ){
            maxCollision = 3.0f;
            MoveTo(s->DoorX(), s->DoorY());
            thought = "Going home";
            return;
        }
        if(GetCarriedItem()){
            taskPtr = s->FindPlaceToDropOff(GetCarriedItem());
            if(SetTransportAssignment(taskPtr)){
                return;
            }
        }
        targetBuildingPtr = s->GetBuildPlace(myPtr);
        if(auto targetBuildingPtrS = targetBuildingPtr.lock()){
            ClearActions();
            thought = "Found a building site";
            targetBuildingPtrS->AddWorker(myPtr);
            assignment = CAction::builderAssignment;
            return;
        }
        workBuildingPtr = s->GetWorkPlace(myPtr);
        if(auto workBuildingPtrS = workBuildingPtr.lock()){
            ClearActions();
            if(workBuildingPtrS->GetResource()){
                assignment = CAction::gatherResources;
                resourceTilePtr.reset();
                workBuildingPtrS->AddWorker(myPtr);
                thought = "Found a gathering assignment";
            }else if(workBuildingPtrS->CanProduce(skillLvl)){
                assignment = CAction::productionAssignment;
                workBuildingPtrS->AddWorker(myPtr);
                thought = "Found a production assignment";
            }
            return;
        }
        taskPtr = s->FindConnectedTask(tileX, tileY);
        if(SetTransportAssignment(taskPtr)){
            return;
        }
        if( GetTileFlightRoundDistance(s->DoorX(), s->DoorY()) < 5 ){
            if(rand() % 5 < 1){
                MoveNextTo(tileX, tileY);
            }
            Idle(60, true);
            thought =  "Idling at home";
            return;
        }
        if(!MoveTo(s->DoorX(), s->DoorY())){
            maxCollision = 3.0f;
            MoveTo(s->DoorX(), s->DoorY());
        }
        thought = "Going home";
        return;
    }
    Idle(600);
    thought = "I have no home...";
}

void CUnit::UpdateTransportAssignment(){
    if(auto taskPtrS = taskPtr.lock()){
        if(GetCarriedItem()){
            if(auto destS = taskPtrS->GetDropOff().lock()){
                if(destS->IsValidWorkLocation(tileX, tileY)){
                    if(!destS->InventoryAvailable()){
                        Idle(10);
                        thought = "Waiting for my turn";
                        return;
                    }
                    if(destS->AddToInventory(GetCarriedItem(), 1) == 0){
                        destS->UseInventory();
                        GetCarriedItem(true);
                        taskPtrS->MarkComplete();
                        taskPtr.reset();
                        thought = "Dropped off goods";
                        return;
                    }
                    thought = "Cant drop off goods!";
                    CancelTransportTask();
                    return;
                }
                vec2i door = destS->GetDoor();
                if(MoveTo(door.first, door.second)){
                    thought = "Going to drop off goods";
                    return;
                }
                thought = "No path to drop off goods";
                CancelTransportTask();
                return;
            }
            CancelTransportTask();
            return;
        }
        if(auto destS = taskPtrS->GetPickUp().lock()){
            if(destS->IsValidWorkLocation(tileX, tileY)){
                if(!destS->InventoryAvailable()){
                    Idle(10);
                    thought = "Waiting for my turn";
                    return;
                }
                if(destS->TakeFromInventory(taskPtrS->GetResource(), 1) == 1){
                    destS->UseInventory();
                    CarryItem(taskPtrS->GetResource());
                    thought = "Picked up goods";
                    return;
                }
                thought = "Cant pick up goods!";
                CancelTransportTask();
                return;
            }
            vec2i randomTile = destS->GetRandomPassableTile();
            if(MoveTo(randomTile.first, randomTile.second)){
                thought = "Going to pick up goods";
                return;
            }
            thought = "No path to pick up goods";
            CancelTransportTask();
            return;
        }
        CancelTransportTask();
        thought =  "My task has no destination";
        return;
    }
    SetIdleAssignment();
    thought = "Going home";
    return;
}

void CUnit::UpdateTraderAssignment(){
    if(auto homeBuildingPtrS = homeBuildingPtr.lock()){
        if(auto targetBuildingPtrS = targetBuildingPtr.lock()){
            // we have a target to reach
            if(GetTileFlightSquareDistance(targetBuildingPtrS->DoorX(), targetBuildingPtrS->DoorY()) < 1 ){
                // we have reached our destination
                int surplus;
                for(auto const& item : Inventory ){
                    if(item.second > 0){
                        surplus = targetBuildingPtrS->AddToInventory(item.first, item.second);
                        Inventory.at(item.first) = surplus;
                    }
                }
                FillTraderInventory(targetBuildingPtr, homeBuildingPtr);
                targetBuildingPtr.reset();
                thought = "Dropped off my cargo";
                return;
            }else{
                // we need to move to another town
                thought = "Going to another town";
                MoveTo(targetBuildingPtrS->DoorX(), targetBuildingPtrS->DoorY());
            }
        }else{
            // we have to go home
            if(GetTileFlightSquareDistance(homeBuildingPtrS->DoorX(), homeBuildingPtrS->DoorY()) < 1 ){
                // we have reached our destination
                int surplus;
                for(auto const& item : Inventory ){
                    if(item.second > 0){
                        surplus = homeBuildingPtrS->AddToInventory(item.first, item.second);
                        Inventory.at(item.first) = surplus;
                    }
                }
                targetBuildingPtr = homeBuildingPtrS->GetNextTraderDestination();
                FillTraderInventory(homeBuildingPtr, targetBuildingPtr);
                thought = "Reached my home";
                return;
            }else{
                // we need to move to another town
                thought = "Going home";
                MoveTo(homeBuildingPtrS->DoorX(), homeBuildingPtrS->DoorY());
            }
        }
    }
}

void CUnit::UpdateFollowAssignment(){
    auto targetUnitPtrS = targetUnitPtr.lock();
    if(!targetUnitPtrS){
        SetIdleAssignment();
        return;
    }
    if(GetTileFlightSquareDistance(targetUnitPtrS->GetTileX(), targetUnitPtrS->GetTileY()) < 5){
        Idle(60);
        thought = "My leader is close by";
        return;
    }
    if(GetTileFlightSquareDistance(targetUnitPtrS->GetTileX(), targetUnitPtrS->GetTileY()) > 10){
        moveSpeed = baseSpeed / 2;
    }
    MoveTo(targetUnitPtrS->GetTileX(), targetUnitPtrS->GetTileY(), 3);
    thought = "Following leader";
    return;
}

void CUnit::UpdateGatherAssignment(){
    auto workBuildingPtrS = workBuildingPtr.lock();
    if(!workBuildingPtrS){
        SetIdleAssignment();
        return;
    }
    maxCollision = 3.0f;
    if(GetCarriedItem()){
        if(GetTileFlightSquareDistance(workBuildingPtrS->DoorX(), workBuildingPtrS->DoorY()) < 1 ){
            if(workBuildingPtrS->AddToInventory(GetCarriedItem(), 1) == 0){
                GetCarriedItem(true);
                thought = "Dropped off goods";
                return;
            }
            thought = "Can't drop off goods at home!";
            return;
        }
        if(!MoveTo(workBuildingPtrS->DoorX(), workBuildingPtrS->DoorY())){
            thought = "Can't find a path to drop off goods!";
            return;
        }
        thought = "Going to drop off goods";
        return;
    }
    if(auto s = resourceTilePtr.lock()){
        if(GetTileFlightSquareDistance(s->GetTileX(), s->GetTileY()) <= 1 ){
            CAction action = CAction(CAction::gatherResource
                                     , GAP.Setting(CSettingManager::ResourceGatherDuration)
                                     , s->GetResource());
            AddAction(action);
            thought = "Gathering resource";
            return;
        }
        if(!MoveNextTo(s->GetTileX(), s->GetTileY())){
            Idle(60);
            resourceTilePtr.reset();
            thought = "Can't reach my resource";
        }
        thought = "Moving to reach resource";
        return;
    }
    int factor = 4;
    while(factor > 0){
        std::vector<tile_weak_ptr> nearestResource = GAP.ChunkManager.GetResources(workBuildingPtrS->GetResource(),
                  workBuildingPtrS->ResourceArea() / factor, workBuildingPtrS->DoorX(), workBuildingPtrS->DoorY());
        if(nearestResource.size() > 0){
            std::random_shuffle ( nearestResource.begin(), nearestResource.end() );
            resourceTilePtr = nearestResource.back();
            thought = "Found a resource";
            return;
        }
        factor--;
    }
    SetIdleAssignment();
    return;
}

void CUnit::UpdateBuildAssignment(){
    if(auto targetBuildingPtrS = targetBuildingPtr.lock()){
        if(GetTileFlightSquareDistance(targetBuildingPtrS->DoorX(), targetBuildingPtrS->DoorY()) < 1 ){
            CAction action = CAction(CAction::buildBuilding,
                                     GAP.Setting(CSettingManager::BuildDuration), 1);
            AddAction(action);
            thought = "Building!";
            return;
        }
        thought = "Going to building site";
        if(!MoveTo(targetBuildingPtrS->DoorX(), targetBuildingPtrS->DoorY())){
            thought = "Can't find a path...";
        }
        return;
    }
    SetIdleAssignment();
    return;
}

void CUnit::UpdateProductionAssignment(){
    auto workBuildingPtrS = workBuildingPtr.lock();
    if(!workBuildingPtrS){
        SetIdleAssignment();
        return;
    }
    if(!workBuildingPtrS->CanProduce(skillLvl)){
        SetIdleAssignment();
        return;
    }
    if(workBuildingPtrS->IsValidWorkLocation(tileX, tileY)){
        CAction action = CAction(CAction::doProduce, 240, 1);
        AddAction(action);
        thought = "Producing!";
        return;
    }
    thought = "Going to production site";
    vec2i randomTile = workBuildingPtrS->GetDoor();
    MoveTo(randomTile.first, randomTile.second);
    return;
}

void CUnit::UpdateAssignment(){
    if(id == 1){
        return;
    }
    moveSpeed = baseSpeed;
    busyTime = 0;
    maxCollision = 2.0f;
    switch(assignment){
    case CAction::idleAssignment:
        UpdateIdleAssignment(); break;
    case CAction::followUnit:
        UpdateFollowAssignment(); break;
    case CAction::gatherResources:
        UpdateGatherAssignment(); break;
    case CAction::builderAssignment:
        UpdateBuildAssignment(); break;
    case CAction::productionAssignment:
        UpdateProductionAssignment(); break;
    case CAction::transportAssignment:
        UpdateTransportAssignment(); break;
    case CAction::traderAssignment:
        UpdateTraderAssignment(); break;
    }
}

void CUnit::CancelTransportTask(){
    if(auto taskPtrS = taskPtr.lock()){
        taskPtrS->DropPorter();
        taskPtr.reset();
    }
}

void CUnit::SetId(int id_, unit_weak_ptr myPtr_){
    id = id_;
    myPtr = myPtr_;
}

bool CUnit::IsIdle(){
    return isIdle;
}

void CUnit::SetHome(build_weak_ptr ptr){
    if(auto s = ptr.lock()){
        homeBuildingPtr = ptr;
    }
}

void CUnit::Destroy(){
    GAP.ChunkManager.MoveUnit(tileX, tileY, 0, 0, myPtr, id, true);
    if(auto s = homeBuildingPtr.lock()){
        s->RemoveInhabitant(id);
    }
    if(auto s = workBuildingPtr.lock()){
        s->RemoveWorker(id);
    }
    GAP.UnitManager.DestroyUnit(id);
}

void CUnit::CarryItem(int resource){
    carriedItem = resource;
}

void CUnit::FillTraderInventory(build_weak_ptr src, build_weak_ptr dest){
    if(auto s = src.lock()){
        if(auto d = dest.lock()){
            int currentInventoryCargo = 0;
            for(auto const& item : Inventory ){
                currentInventoryCargo += item.second;
            }
            while(currentInventoryCargo < inventorySize){
                int biggestDelta = 1;
                int biggestItem = 0;
                int delta = 0;
                for(auto const& item : *(s->GetInventory()) ){
                    delta = item.second - d->GetInventory()->at(item.first);
                    if(Inventory.at(item.first) > 0){
                        delta = delta / Inventory.at(item.first);
                    }
                    if(delta > biggestDelta){
                        biggestDelta = delta;
                        biggestItem = item.first;
                    }
                }
                if(!biggestItem){
                    return;
                }
                if(s->TakeFromInventory(biggestItem, 1) == 1){
                    Inventory.at(biggestItem)++;
                    currentInventoryCargo++;
                }else{
                    return;
                }
            }
        }
    }
}

int CUnit::GetCarriedItem(bool takeIt){
    int item = carriedItem;
    if(takeIt){
        carriedItem = 0;
    }
    return item;
}

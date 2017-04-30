#include "CUnit.h"
#include "CGame.h"

extern CGame GAP;
typedef std::weak_ptr<CUnit> unit_weak_ptr;

CUnit::CUnit(int x, int y, std::string myName){
    this->tileX = x;
    this->x = x * CScreen::tileWidth ;
    this->tileY = y;
    this->y = y * CScreen::tileWidth ;
    z = 1000 + (rand() % 1000);
    z = 1000 + (rand() % 1000);
    xoff = - ((CScreen::unitWidth - CScreen::tileWidth) / 2);
    yoff = - (CScreen::unitWidth / 2);
    name = myName;
    box.h = CScreen::unitWidth;
    box.w = CScreen::unitWidth;
    moveSpeed = baseSpeed = 16;
    currentFrame = 0;
    currentFrameStarted = 0;
    facing = 2;
    moveDirection = 4;
    currentAnimation = 7;
    moving = false;
    busyTime = 0;
    minCollision = 0.0f;
    maxCollision = 3.0f;
    assignment = CAction::idleAssignment;

    this->resetFrames();
}

CUnit::~CUnit(){
    //dtor
}

bool CUnit::Render(){

    box.x = this->x + xoff;
    box.y = this->y + yoff;
    if(currentFrameStarted + currentFrameSpeed < GAP.getTick() ){
        currentFrameStarted = GAP.getTick();
        currentFrame++;
        if(currentFrame >= frameCount){
            currentFrame = 0;
        }
    }
    int frame = currentFrame + frameOffset;

    GAP.TextureManager.DrawTextureGL(name, GAP.TextureManager.GetClip(frame), &box);
    return 1;
}

void CUnit::Update(){

    if(!busyTime){
        if(!ActionQueue.size()){
            UpdateAssignment();
        }
        if(!ActionQueue.size()){
            SetIdleAssignment();
        }
        if(ActionQueue.size()){
            isIdle = false;
            currentAction = ActionQueue.front();
            ActionQueue.erase (ActionQueue.begin());
            busyTime = ExecuteAction(currentAction);
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
    busyTime--;
}

std::string CUnit::getName() const{
    return name;
}

void CUnit::resetFrames(){
    setFrameCount();
    setFrameOffset();
    setFrameSpeed();
    x = GAP.tileToPixel(tileX);
    y = GAP.tileToPixel(tileY);
    currentFrame = 0;
    currentFrameStarted = GAP.getTick();
}

void CUnit::setFacing(int direction){
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
        this->resetFrames();
    }
    moveDirection = direction;
}

void CUnit::setMoving(bool isMoving){
    if(isMoving){
        this->setAnimation("walking");
    }
    else{
        this->setAnimation("standing");
    }
    moving = isMoving;
}

void CUnit::setAnimation(std::string animation){
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
        this->resetFrames();
    }
}

int CUnit::setFrameCount(){
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

int CUnit::setFrameSpeed(){
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

int CUnit::setFrameOffset(){
    if(currentAnimation == 6){
        this->frameOffset = this->baseOffset();
    }else{
        this->frameOffset = this->baseOffset() + (13 * facing);
    }
    return this->frameOffset;
}

int CUnit::baseOffset(){
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
                if(s->getResource()){
                    StopMovement();
                    setAnimation("slashing");
                    setFacing(getTileDirection(s->getTileX(), s->getTileY()));
                    timeForAction = action_.arga;
                }
            }
            break;
        case CAction::dropGoods:
            StopMovement();
            BackPack.clear();
            timeForAction = action_.arga;
            break;
        case CAction::buildBuilding:
            StopMovement();
            setAnimation("slashing");
            timeForAction = action_.arga;
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
                    BackPack.push_back(action_.argb);
                    if(!s->getResource()){
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
    }
    return 0;
}

void CUnit::StopMovement(){
    resetFrames();
    isIdle = true;
    setMoving(false);
}

bool CUnit::MoveToAdyacent(int x_, int y_){
    if(getTileFlightSquareDistance(x_, y_) > 1 || (x == x_  && y == y_)){
        return false;
    }
    float moveCost = GAP.Pathfinder.GetCost(x_, y_);
    if(GAP.Pathfinder.GetCost(x_, y_) >= GetMaxCollision()){
        return false;
    }
    moveSpeed = baseSpeed;
    if(moveCost < 2.0f){
        moveSpeed = 11;
    }
    if(moveCost < 1.0f){
        moveSpeed = 8;
    }
    setMoving(true);
    setFacing(getTileDirection(x_, y_));
    GAP.ChunkManager.MoveUnit(tileX, tileY, x_, y_, myPtr, id);
    tileX = x_;
    tileY = y_;
    carryOverPixels = 0.0f;
    return true;
}

float CUnit::getMoveSpeed(){
    return ( (float)CScreen::tileWidth / (float)moveSpeed ) ;
}

float CUnit::GetMinCollision(){
    return minCollision;
}

float CUnit::GetMaxCollision(){
    return maxCollision;
}

void CUnit::ClearActions(){
    ActionQueue.clear();
}

bool CUnit::MoveTo(int x, int y, int stopShort){
    ClearActions();
    CoordList path = GAP.Pathfinder.FindPath(
        Coord( getTileX(), getTileY() ),
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
            Coord( getTileX(), getTileY() ),
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

void CUnit::Idle(int time){
    ClearActions();
    CAction action = CAction(CAction::idleAction, time, 0);
    AddAction(action);
}

void CUnit::SetIdleAssignment(){
    ClearActions();
    assignment = CAction::idleAssignment;
    UpdateAssignment();
}

void CUnit::SetFollowAssignment(unit_weak_ptr targetPtr_){
    ClearActions();
    if(targetPtr_.lock()){
        targetUnitPtr = targetPtr_;
        assignment = CAction::followUnit;
    }
    UpdateAssignment();
}

void CUnit::SetGatherAssignment(build_weak_ptr targetPtr_){
    ClearActions();
    if(targetPtr_.lock()){
        workBuildingPtr = targetPtr_;
        resourceTilePtr.reset();
        assignment = CAction::gatherResources;
    }
    UpdateAssignment();
}

void CUnit::SetBuildAssignment(build_weak_ptr targetPtr_){
    ClearActions();
    if(targetPtr_.lock()){
        workBuildingPtr = targetPtr_;
        targetBuildingPtr.reset();
        assignment = CAction::builderAssignment;
    }
    UpdateAssignment();
}

void CUnit::UpdateAssignment(){
    moveSpeed = baseSpeed;
    switch(assignment){
    case CAction::idleAssignment:
        if(auto s = homeBuildingPtr.lock()){
            if(s->getDoor().first == tileX && s->getDoor().second == tileY){
                Idle(600);
                CLog::Write("Idling at my door...");
                return;
            }
            MoveTo(s->getDoor().first, s->getDoor().second);
            CLog::Write("Moving to idle at home...");
            return;
        }
        Idle(600);
        break;
    case CAction::followUnit:{
        CLog::Write("Processing FollowUnit Assignment");
        auto targetUnitPtrS = targetUnitPtr.lock();
        if(!targetUnitPtrS){
            SetIdleAssignment();
            return;
        }
        if(getTileFlightSquareDistance(targetUnitPtrS->getTileX(), targetUnitPtrS->getTileY()) < 5){
            Idle(60);
            return;
        }
        if(getTileFlightSquareDistance(targetUnitPtrS->getTileX(), targetUnitPtrS->getTileY()) > 10){
            moveSpeed = baseSpeed / 2;
        }
        MoveTo(targetUnitPtrS->getTileX(), targetUnitPtrS->getTileY(), 3);
        return;
    }
    case CAction::gatherResources:{
        CLog::Write("Processing Gathering Assignment");
        auto workBuildingPtrS = workBuildingPtr.lock();
        if(!workBuildingPtrS){
            SetIdleAssignment();
            CLog::Write("I have no workplace!?");
            return;
        }
        if(BackPack.size() > 10){
            if(getTileFlightSquareDistance(workBuildingPtrS->getDoor().first, workBuildingPtrS->getDoor().second) < 1 ){
                DropBackpack();
                return;
            }
            MoveTo(workBuildingPtrS->getDoor().first, workBuildingPtrS->getDoor().second);
            return;
        }
        if(auto s = resourceTilePtr.lock()){
            if(getTileFlightSquareDistance(s->getTileX(), s->getTileY()) <= 1 ){
                GatherResource(s->getResource());
                return;
            }
            if(!MoveNextTo(s->getTileX(), s->getTileY())){
                Idle(120);
                hasTargetTile = false;
                return;
            }
            return;
        }
        int factor = 4;
        while(factor > 0){
            std::vector<tile_weak_ptr> nearestResource = GAP.ChunkManager.GetResources(workBuildingPtrS->getResource(),
                      workBuildingPtrS->ResourceArea() / factor, workBuildingPtrS->getDoor().first, workBuildingPtrS->getDoor().second);
            if(nearestResource.size() > 0){
                std::random_shuffle ( nearestResource.begin(), nearestResource.end() );
                resourceTilePtr = nearestResource.back();
                CLog::Write("Found a resource");
                Idle(1);
                return;
            }
            factor--;
        }
        SetIdleAssignment();
        return;
    }
    case CAction::builderAssignment:
        CLog::Write("Processing Building Assignment");
        auto workBuildingPtrS = workBuildingPtr.lock();
        if(!workBuildingPtrS){
            CLog::Write("I have no workplace!?");
            SetIdleAssignment();
            return;
        }
        if(auto targetBuildingPtrS = targetBuildingPtr.lock()){
            CLog::Write("Going to work at site");
            if(getTileFlightSquareDistance(targetBuildingPtrS->getDoor().first, targetBuildingPtrS->getDoor().second) < 1 ){
                CAction action = CAction(CAction::buildBuilding, 240, 1);
                AddAction(action);
                return;
            }
            MoveTo(targetBuildingPtrS->getDoor().first, targetBuildingPtrS->getDoor().second);
            return;
        }
        std::vector<build_weak_ptr> nearestSites = GAP.BuildingManager.GetUnfinishedBuildings(
                              workBuildingPtrS->BuildArea() , workBuildingPtrS->getDoor().first, workBuildingPtrS->getDoor().second);
        if(nearestSites.size() > 0){
            std::random_shuffle ( nearestSites.begin(), nearestSites.end() );
            targetBuildingPtr = nearestSites.back();
            CLog::Write("Found a site");
            Idle(1);
            return;
        }
        if(getTileFlightSquareDistance(workBuildingPtrS->getDoor().first, workBuildingPtrS->getDoor().second) < 1 ){
            Idle(60);
            CLog::Write("Nothing to build");
            return;
        }
        MoveTo(workBuildingPtrS->getDoor().first, workBuildingPtrS->getDoor().second);
        CLog::Write("Nothing to build, going home");
        return;
    }
}

void CUnit::setId(int id_, unit_weak_ptr myPtr_){
    id = id_;
    myPtr = myPtr_;
}

unit_weak_ptr CUnit::GetPtr(){
    return myPtr;
}

int CUnit::getAssignment(){
    return assignment;
}

void CUnit::GatherResource(int resource){
    switch(resource){
    case CAction::wood:
        CAction action = CAction(CAction::gatherResource, 240, 1);
        AddAction(action);
        break;
    }
}

void CUnit::DropBackpack(){
    CAction action = CAction(CAction::dropGoods, 1, 1);
    AddAction(action);
}

bool CUnit::IsIdle(){
    return isIdle;
}

int CUnit::GetBusyTime(){
    return busyTime;
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
    if(auto s = targetBuildingPtr.lock()){
        s->RemoveWorker(id);
    }
    GAP.UnitManager.DestroyUnit(id);
}


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
    healthBarMaxWidth = CScreen::tileWidth;
    itemBox.h = CScreen::tileWidth;
    itemBox.w = CScreen::tileWidth;
    assignment = CAction::idleAssignment;
    for(auto p : CGood::GetResources() ){
        Inventory[p.first] = 0;
    }
    this->ResetFrames();
	
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
    if(currentHealth < maxHealth){
        GAP.TextureManager.DrawRectangle(x, box.y, x + healthBarWidth, box.y + 5, healthBarColor );
    }
    return 1;
}

bool CUnit::RenderInPosition(GPU_Rect* destBox){
    GAP.TextureManager.DrawTextureGL(&name, GAP.TextureManager.GetClip(currentFrame + frameOffset), destBox, true);
    return 1;
}

void CUnit::Update(){

    if( ! ((id + GAP.GetTick()) % 60) ){
        UpdateNearbyUnits();
    }
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
            currentFrameSpeed = moveSpeed / 2;
            break;
        default:
            currentFrameSpeed = 500 / attackSpeed;
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
        case CAction::doSlash:
            StopMovement();
            if(auto s = targetUnitPtr.lock()){
                SetAnimation("slashing");
                timeForAction = currentFrameSpeed * frameCount;
                SetFacing(GetTileDirection(s->GetTileX(), s->GetTileY()));
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
        case CAction::doSlash:
            GAP.AttackManager.AddAttack(myPtr, targetUnitPtr, attackDamage);
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
    if(auto s = GAP.ChunkManager.GetTile(x_, y_).lock()){
        if(s->Owner() > 0 && s->Owner() != owner){
            return false;
        }
    }
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

void CUnit::SetFollowAssignment(unit_weak_ptr targetPtr_){
    ClearActions();
    if(targetPtr_.lock()){
        targetUnitPtr = targetPtr_;
        assignment = CAction::followUnit;
    }
    maxCollision = 3.0f;
    UpdateAssignment();
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

int CUnit::GetCarriedItem(bool takeIt){
    int item = carriedItem;
    if(takeIt){
        carriedItem = 0;
    }
    return item;
}

void CUnit::UpdateNearbyUnits(){
    NearbyHostiles = GAP.UnitManager.FindHostileUnitsWithinRange(tileX, tileY, CScreen::tilesPerChunk, owner);
}

void CUnit::DoDamage(int amount){
    currentHealth -= amount;
    if(currentHealth < 1){
        Destroy();
    }
    int halfHealth = (maxHealth / 2);
    if(currentHealth > halfHealth){
        healthBarColor = {100, static_cast<Uint8>( 100 + ( (155 * (currentHealth - halfHealth)) / halfHealth) ), 100, 255};
    }else{
        healthBarColor = { static_cast<Uint8>( 255 - ( (155 * currentHealth ) / halfHealth) ), 100 , 100, 255};
    }
    healthBarWidth = (healthBarMaxWidth * currentHealth) / maxHealth;
}

void CUnit::LoadLine(std::string key, std::string value){

	if(!key.compare("health")){
		currentHealth = maxHealth = std::stoi(value) * 10;
		return;
	}
	if(!key.compare("armor")){
		armor = std::stoi(value) * 10;
		return;
	}
	if(!key.compare("attackDamage")){
		attackDamage = std::stoi(value) * 10;
		return;
	}
	if(!key.compare("attackSpeed")){
		attackSpeed = std::stoi(value) * 10;
		return;
	}
	if(!key.compare("detectionDistance")){
		detectionDistance = std::stoi(value);
		return;
	}
	if(!key.compare("baseSpeed")){
		baseSpeed = std::stoi(value);
		return;
	}
}



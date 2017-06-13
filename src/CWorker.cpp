#include "CWorker.h"
#include "CGame.h"

extern CGame GAP;

bool CWorker::SetTransportAssignment(task_weak_ptr ptr){
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

void CWorker::SetTraderAssignment(){
    ClearActions();
    assignment = CAction::traderAssignment;
    maxCollision = 2.0f;
    UpdateAssignment();
}

void CWorker::UpdateIdleAssignment(){
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

void CWorker::UpdateTransportAssignment(){
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

void CWorker::UpdateTraderAssignment(){
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

void CWorker::UpdateFollowAssignment(){
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

void CWorker::UpdateGatherAssignment(){
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

void CWorker::UpdateBuildAssignment(){
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

void CWorker::UpdateProductionAssignment(){
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

void CWorker::UpdateAssignment(){
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

void CWorker::FillTraderInventory(build_weak_ptr src, build_weak_ptr dest){
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

void CWorker::CancelTransportTask(){
    if(auto taskPtrS = taskPtr.lock()){
        taskPtrS->DropPorter();
        taskPtr.reset();
    }
}


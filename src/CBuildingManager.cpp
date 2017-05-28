#include "CBuildingManager.h"
#include "CGame.h"

extern CGame GAP;

bool CBuildingManager::Init(){
    CLog::Write("Loading Buildings");
    DIR* dir = opendir("defines/buildings");
    if (!dir){
        CLog::Write("Couldnt open saves directory");
        return false;
    }

    struct dirent* entry;
    std::string fileName = "";
    while( (entry = readdir(dir)) != NULL ){
        if(entry->d_namlen < 3) continue;
        fileName = "defines/buildings/";
        fileName.append(entry->d_name);
        LoadBuildingType(fileName);
    }
    return true;
}

void CBuildingManager::LoadBuildingType(std::string fileName){
    CLog::Write(fileName);

	CBuildingType buildingType = CBuildingType ();
	std::ifstream is_file(fileName);
	std::string line = "";

    if (is_file.is_open()){
        while( std::getline(is_file, line) ) {
            std::istringstream is_line(line);
            std::string key = "";
            std::string value = "";
            std::getline(is_line, key, '=');
            std::getline(is_line, value);
            buildingType.LoadLine(key, value);
        }
	}

    std::string gfxPath = "gfx/buildings/";
    gfxPath.append(*(buildingType.GetName()));
    gfxPath.append(".png");

    GAP.TextureManager.LoadTextureGL(*(buildingType.GetName()), gfxPath);

	BuildingTypes.push_back(buildingType);
}

build_weak_ptr CBuildingManager::AddBuilding(int type, int x, int y, int owner, bool ignoreBlocked){

    build_weak_ptr ptr;
    build_shared_ptr building = std::make_shared<CBuilding>(type, x, y, owner);
    std::map<int, int> inventory;

    if(auto s = GetBuildingAt(x, y).lock()){
        if(s->GetTileX() != building->GetTileX() || s->GetTileY() != building->GetTileY() || building->GetName()->compare(*s->GetUpgrade())){
            return ptr;
        }else{
            inventory = s->GetInventory();
            DemolishBuilding(x, y);
        }
    }

    if(building->IsInBlockedLocation()){
        if(!ignoreBlocked){
            return ptr;
        }
    }
    ptr = build_weak_ptr(building);

    buildingCount++;
    building->SetId(buildingCount, ptr);
    building->ApplyMovementCosts();
    for(auto item : inventory){
        building->AddToInventory(item.first, item.second);
    }
	Buildings.push_back(building);
	if(building->DistributionRange()){
        Towns.push_back(build_weak_ptr(building));
	}
	return ptr;
}

void CBuildingManager::Render(){
    for(auto b : Buildings){
        b->Render();
    }
}

CBuildingType* CBuildingManager::GetBuildingType(int type){
    return &BuildingTypes.at(type);
}

std::vector<CBuildingType> CBuildingManager::GetBuildingTypes(){
    return BuildingTypes;
}

void CBuildingManager::DemolishBuilding(int x, int y){
    std::vector<build_shared_ptr>::iterator iter = Buildings.begin();
    while (iter != Buildings.end())
    {
        if( x >= (*iter)->GetTileX() && x < (*iter)->GetTileX() + ((*iter)->GetW() / CScreen::tileWidth)
           && y >= (*iter)->GetTileY() && y < (*iter)->GetTileY() + ((*iter)->GetH() / CScreen::tileWidth) ){
            (*iter)->Destroy();
            Buildings.erase(iter);
            return;
        }
        ++iter;
    }
}

build_weak_ptr CBuildingManager::GetBuildingAt(int x, int y){
    for(auto b : Buildings){
        if(x < b->GetTileX()){
            continue;
        }
        if(y < b->GetTileY()){
            continue;
        }
        if(x >= b->GetTileX() + b->GetTileWidth()){
            continue;
        }
        if(y >= b->GetTileY() + b->GetTileHeight()){
            continue;
        }
        return build_weak_ptr(b);
    }
    return build_weak_ptr();
}

build_weak_ptr CBuildingManager::GetBuilding(int id){
    return build_weak_ptr(Buildings.at(id));
}

void CBuildingManager::Update(){
    if((GAP.GetTick() % CScreen::buildingUpdateFrequency) == 0){
        for(auto e : Buildings){
            e->Update();
        }
        for(auto w : Towns){
            if(auto s = w.lock()){
                s->MatchTransportTasks();
                s->UpdateWorkPositions();
            }
        }
    }
}

void CBuildingManager::UpdateConnections(){

    for(auto e : Buildings){
        e->ConnectToNearestTown();
    }
}

build_weak_ptr CBuildingManager::FindCollision(int x, int y){
    for(auto e : Buildings)
    {
        if(e->FindCollision(x, y)){
            return build_weak_ptr(e);
        }
    }
    build_weak_ptr emptyPtr;
    return emptyPtr;
}





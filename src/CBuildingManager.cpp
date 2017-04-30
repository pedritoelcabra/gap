#include "CBuildingManager.h"
#include "CGame.h"

extern CGame GAP;

CBuildingManager::CBuildingManager(){
    buildingCount = 1;
}

CBuildingManager::~CBuildingManager(){
    //dtor
}

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
    gfxPath.append(buildingType.GetName());
    gfxPath.append(".png");

    GAP.TextureManager.LoadTextureGL(buildingType.GetName(), gfxPath);

	BuildingTypes.push_back(buildingType);
}

build_weak_ptr CBuildingManager::AddBuilding(int type, int x, int y, int owner){

    build_weak_ptr ptr;
    CBuildingType* buildingType = GetBuildingType(type);
    build_shared_ptr building = std::make_shared<CBuilding>(type, x, y, owner);
    if(building->isInBlockedLocation()){
        return ptr;
    }
    ptr = build_weak_ptr(building);
    auto layout = buildingType->GetLayout();
    for(int i = 0; i < building->getH() / CScreen::tileWidth; i++ ){
        for(int k = 0; k < building->getW() / CScreen::tileWidth; k++ ){
            GAP.Pathfinder.SetCost(x + k, y + i, layout.at(i).at(k));
        }
    }
    buildingCount++;
    building->SetId(buildingCount, ptr);
	Buildings.push_back(building);
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
        if( x >= (*iter)->getTileX() && x < (*iter)->getTileX() + ((*iter)->getW() / CScreen::tileWidth)
           && y >= (*iter)->getTileY() && y < (*iter)->getTileY() + ((*iter)->getH() / CScreen::tileWidth) ){
            for(int i = 0; i < ((*iter)->getH() / CScreen::tileWidth); i++ ){
                for(int k = 0; k < ((*iter)->getW() / CScreen::tileWidth); k++ ){
                    tile_shared_ptr tile = GAP.ChunkManager.GetTile( (*iter)->getTileX() + k, (*iter)->getTileY() + i).lock();
                    if(tile){
                        GAP.Pathfinder.SetCost((*iter)->getTileX() + k, (*iter)->getTileY() + i, tile->getMoveCost() );
                    }
                }
            }
            (*iter)->Destroy();
            Buildings.erase(iter);
            return;
        }
        ++iter;
    }
}

build_weak_ptr CBuildingManager::GetBuildingAt(int x, int y){
    for(auto b : Buildings){
        if(x < b->getTileX()){
            continue;
        }
        if(y < b->getTileY()){
            continue;
        }
        if(x > b->getTileX() + b->GetTileWidth()){
            continue;
        }
        if(y > b->getTileY() + b->GetTileHeight()){
            continue;
        }
        return build_weak_ptr(b);
    }
    return build_weak_ptr();
}

std::vector<build_weak_ptr> CBuildingManager::GetUnfinishedBuildings( int radius, int x, int y){
    std::vector<build_weak_ptr> result;
    for(auto b : Buildings){
        if(b->UnderConstruction()){
            if(b->getTileFlightRoundDistance(x, y) <= radius){
                result.push_back(build_weak_ptr(b));
            }
        }
    }
    return result;
}

build_weak_ptr CBuildingManager::GetBuilding(int id){
    return build_weak_ptr(Buildings.at(id));
}

void CBuildingManager::Update(){

    for(auto e : Buildings){
        if(e->UnderConstruction()){
            continue;
        }
        e->Update();
        if(!e->HasEnoughWorkers()){

            for(auto f : Buildings){

                if(f->MaxPop() > 0){

                    if(f->InRadius(e->getTileX(), e->getTileY())){

                        if(auto s = f->GetIdleInhabitant().lock()){
                            if(e->getResource()){
                                s->SetGatherAssignment(build_weak_ptr(e));
                            }
                            if(e->BuildArea()){
                                s->SetBuildAssignment(build_weak_ptr(e));
                            }
                            e->AddWorker(unit_weak_ptr(s));
                            break;
                        }
                    }
                }
            }
        }
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



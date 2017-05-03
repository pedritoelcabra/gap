#include "CBuildingType.h"
#include "CScreen.h"

CBuildingType::CBuildingType(){
    w = -1;
    h = -1;
    draggable = false;
    connects = false;
    resource = 0;
    popMax = 0;
    popCost = 0;
    popRange = 0;
    workerCount = 0;
    buildArea = 0;
    resourceRadius = 0;
    distributionRange = 0;
    //ctor
}

CBuildingType::~CBuildingType(){
    //dtor
}

void CBuildingType::LoadLine(std::string key, std::string value){

	if(!key.compare("name")){
		name = value;
		return;
	}
	if(!key.compare("resourceGatherer")){
		resource = std::stoi(value);
		return;
	}
	if(!key.compare("populationMax")){
		popMax = std::stoi(value);
		return;
	}
	if(!key.compare("populationCost")){
		popCost = std::stoi(value);
		return;
	}
	if(!key.compare("populationRange")){
		popRange = std::stoi(value);
		return;
	}
	if(!key.compare("distributionRange")){
		distributionRange = std::stoi(value);
		return;
	}
	if(!key.compare("transportRange")){
		transportRange = std::stoi(value);
		return;
	}
	if(!key.compare("workerCount")){
		workerCount = std::stoi(value);
		return;
	}
	if(!key.compare("workerPriority")){
		workerPriority = std::stoi(value);
		return;
	}
	if(!key.compare("buildArea")){
		buildArea = std::stoi(value);
		return;
	}
	if(!key.compare("resourceRadius")){
		resourceRadius = std::stoi(value);
		return;
	}
	if(!key.compare("draggable")){
		draggable = true;
		return;
	}
	if(!key.compare("connects")){
		connects = true;
		return;
	}
	if(!key.compare("description")){
		description = value;
		return;
	}
	if(!key.compare("upgrade")){
		upgrade = value;
		return;
	}
	if(!key.compare("buildCost")){
		LoadCosts(value);
		return;
	}
	if(!key.compare("buildRequirement")){
		Requirements.push_back(value);
		return;
	}
	if(!key.compare("layoutLine")){
		LoadLayout(value);
		return;
	}
}

void CBuildingType::LoadCosts(std::string value){

	std::istringstream is_value(value);
	std::string type;
	if( std::getline(is_value, type, ':') ) {
		std::string amount;
		if( std::getline(is_value, amount) ) {
            if(type.compare("wood") == 0){
                BuildCosts[CGood::wood] = std::stoi( amount );
            }
            if(type.compare("stone") == 0){
                BuildCosts[CGood::stone] = std::stoi( amount );
            }
            if(type.compare("work") == 0){
                BuildCosts[CGood::work] = std::stoi( amount );
            }
		}
	}

}

void CBuildingType::LoadLayout(std::string value){

	std::istringstream is_value(value);
	std::string place;
	std::vector<float> layoutLine;
	while( std::getline(is_value, place, ':') ) {
		layoutLine.push_back(std::stof( place ));
	}

	Layout.push_back(layoutLine);
	InitLayout();
}

void CBuildingType::InitLayout(){
    h = Layout.size() * CScreen::tileWidth;
    w = 0;
    for(std::vector<float> v : Layout){
        if((int)v.size() > w){
            w = v.size();
        }
    }
    w = w * CScreen::tileWidth;
    for(int i = 0; i < h / CScreen::tileWidth; i++ ){
        for(int k = 0; k < w / CScreen::tileWidth; k++ ){
            if(Layout.at(i).at(k) == 0.0f){
                door.first = k;
                door.second = i;
            }
        }
    }
    clip.x = 0;
    clip.y = 0;
    clip.h = h;
    clip.w = w;
}

int CBuildingType::GetCost(int type){
    if (BuildCosts.count(type)){
        return BuildCosts.at(type);
    }
    return 0;
}


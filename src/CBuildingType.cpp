#include "CBuildingType.h"
#include "CScreen.h"
#include "CGame.h"

extern CGame GAP;

CBuildingType::CBuildingType(){
}

CBuildingType::~CBuildingType(){
}

void CBuildingType::LoadLine(std::string key, std::string value){

	if(!key.compare("name")){
		name = value;
		return;
	}
	if(!key.compare("resourceGatherer")){
		resource = CGood::GetResourceByName(value);
		return;
	}
	if(!key.compare("maxStorage")){
		maxStorage = std::stoi(value);
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
	if(!key.compare("randomWorkPosition")){
		randomWorkPosition = true;
		return;
	}
	if(!key.compare("productionStages")){
		productionStages = std::stoi(value);
		return;
	}
	if(!key.compare("productionCooldown")){
		productionCooldown = std::stoi(value);
		return;
	}
	if(!key.compare("productionSetback")){
		productionSetback = std::stoi(value);
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
	if(!key.compare("productionRecipe")){
		Recipes.push_back(GAP.RecipeManager.GetRecipeByName(value));
		return;
	}
	if(!key.compare("buildCost")){
		LoadCosts(BuildCosts, value);
		return;
	}
	if(!key.compare("techRequirement")){
		Requirements.push_back(GAP.TechManager.GetTechByName(&value)->GetId());
		return;
	}
	if(!key.compare("layoutLine")){
		LoadLayout(value);
		return;
	}
	if(!key.compare("inhabitant")){
		inhabitant = value;
		return;
	}
	if(!key.compare("inhabitantSkill")){
		inhabitantSkill = std::stoi(value);
		return;
	}
}

int CBuildingType::ConsumesResource(int res_){
	int mostResource = 0;
	for(auto r : Recipes){
		if(r.ConsumesResource(res_) > mostResource){
			mostResource = r.ConsumesResource(res_);
		}
	}
    	return mostResource;
}

int CBuildingType::ProducesResource(int res_){
	int mostResource = 0;
	for(auto r : Recipes){
		if(r.ProducesResource(res_) > mostResource){
			mostResource = r.ProducesResource(res_);
		}
	}
    	return mostResource;
}

void CBuildingType::LoadCosts(std::map< int, int > & container, std::string value){

	std::istringstream is_value(value);
	std::string type;
	if( std::getline(is_value, type, ':') ) {
		std::string amount;
		if( std::getline(is_value, amount) ) {
		    container[CGood::GetResourceByName(type)] = std::stoi( amount );
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

int CBuildingType::BuildCost(int res_){
    if(BuildCosts.count(res_)){
	return BuildCosts.at(res_);
    }
    return 0;
};

bool CBuildingType::RequirementsMet(){
    for(auto t : Requirements){
        if(!GAP.TechManager.IsResearched(t)){
            return false;
        }
    }
    return true;
}


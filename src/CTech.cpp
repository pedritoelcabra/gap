#include "CTech.h"
#include "CGame.h"

extern CGame GAP;

void CTech::LoadLine(std::string key, std::string value){

	if(!key.compare("name")){
		name = value;
		return;
	}
	if(!key.compare("description")){
		description = value;
		return;
	}
	if(!key.compare("minSkill")){
		minSkill = std::stoi(value);
		return;
	}
	if(!key.compare("techCost")){
		techCost = std::stoi(value);
		return;
	}

	if(!key.compare("resourceInput")){
		LoadCosts(Input, value);
		return;
	}
	if(!key.compare("techRequirement")){
    RequirementsNames.push_back(value);
		return;
	}
}

void CTech::LoadCosts(std::map< int, int > & container, std::string value){

	std::istringstream is_value(value);
	std::string type;
	if( std::getline(is_value, type, ':') ) {
		std::string amount;
		if( std::getline(is_value, amount) ) {
            for(auto const &item : CGood::GetResources()){
                if(type.compare(item.second) == 0){
                    container[item.first] = std::stoi( amount );
                }
            }
		}
	}

}

bool CTech::AddProgress(){
    currentProgress++;
    if(currentProgress >= techCost){
        isResearched = true;
    }
    return isResearched;
}

void CTech::Init(){
    for(auto n : RequirementsNames){
        Requirements.push_back(GAP.TechManager.GetTechByName(&n)->GetId());
    }
    if(GAP.Setting(CSettingManager::AllTechsResearched)){
        isResearched = true;
    }
}

void CTech::UpdateStatus(){
    if(isResearched){
        return;
    }
    if(isAvailable){
        return;
    }
    for(auto i : Requirements){
        if(!GAP.TechManager.GetTechById(i)->IsResearched()){
            return;
        }
    }
    isAvailable = true;
}


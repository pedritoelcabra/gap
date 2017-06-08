#include "CRecipe.h"
#include "CGame.h"

extern CGame GAP;

void CRecipe::LoadLine(std::string key, std::string value){

	if(!key.compare("name")){
		name = value;
		return;
	}
	if(!key.compare("minSkill")){
		minSkill = std::stoi(value);
		return;
	}
	if(!key.compare("resourceInput")){
		LoadCosts(Input, value);
		return;
	}
	if(!key.compare("resourceOutput")){
		LoadCosts(Output, value);
		return;
	}
	if(!key.compare("techRequirement")){
    RequirementsNames.push_back(value);
		return;
	}
}

int CRecipe::ConsumesResource(int res_){
    if(res_ == 0){
        return Input.size();
    }
    if (Input.count(res_)){
        return Input.at(res_);
    }
    return 0;
}

int CRecipe::ProducesResource(int res_){
    if(res_ == 0){
        return Output.size();
    }
    if (Output.count(res_)){
        return Output.at(res_);
    }
    return 0;
}

void CRecipe::LoadCosts(std::map< int, int > & container, std::string value){

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

bool CRecipe::IsAvailable(){
    if(parentRecipe != nullptr){
        return parentRecipe->IsAvailable();
    }
    return isAvailable;
}

bool CRecipe::UpdateAvailability(){
    if(isAvailable){
        return true;
    }
    if(!name.compare("PeasantResearch")){
        if(isAvailable){
            CLog::Write("Peasant available");
        }else{
            CLog::Write("Peasant not available");
        }
    }
    for(auto t : Requirements){
        if(!GAP.TechManager.IsResearched(t)){
            return false;
        }
    }
    isAvailable = true;
    return true;
}

void CRecipe::Init(){
    for(auto n : RequirementsNames){
        Requirements.push_back(GAP.TechManager.GetTechByName(&n)->GetId());
    }
    UpdateAvailability();
}

void CRecipe::SetParent(CRecipe* parent_){
    parentRecipe = parent_;
    Requirements.clear();
    RequirementsNames.clear();
}


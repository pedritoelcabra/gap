#include "CTech.h"

void CRecipe::LoadLine(std::string key, std::string value){

	if(!key.compare("name")){
		name = value;
		return;
	}
	if(!key.compare("description")){
		description = value;
		return;
	}
	if(!key.compare("minSkill")){
		minSkill = value;
		return;
	}
	if(!key.compare("techCost")){
		techCost = value;
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
#include "CRecipe.h"

CBuildingType::CRecipe(){
}

CBuildingType::~CRecipe(){
}

void CRecipe::LoadLine(std::string key, std::string value){

	if(!key.compare("name")){
		name = value;
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

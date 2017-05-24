#include "CSettingManager.h"

bool CSettingManager::Init(){
    std::vector<int> settingsInt (100);

    std::string fileName = "defines/defines.txt";
	std::ifstream is_file(fileName);
	std::string line = "";

    if (is_file.is_open()){
        while( std::getline(is_file, line) ) {
            std::istringstream is_line(line);
            std::string key = "";
            std::string value = "";
            std::getline(is_line, key, '=');
            std::getline(is_line, value);

            if(!key.compare("ChunkGenerationDistance")){
                settingsInt.at(ChunkGenerationDistance) = std::stoi(value);
            }
		
            if(!key.compare("BuildingInventoryUseCooldown")){
                settingsInt.at(BuildingInventoryUseCooldown) = std::stoi(value);
            }
		
            if(!key.compare("GathererBuildingStorage")){
                settingsInt.at(BuildingInventoryUseCooldown) = std::stoi(value);
            }
		
            if(!key.compare("ProductionBuildingInputStorage")){
                settingsInt.at(BuildingInventoryUseCooldown) = std::stoi(value);
            }
		
            if(!key.compare("ProductionBuildingOutputStorage")){
                settingsInt.at(BuildingInventoryUseCooldown) = std::stoi(value);
            }
		
            if(!key.compare("BuildDuration")){
                settingsInt.at(BuildingInventoryUseCooldown) = std::stoi(value);
            }
		
            if(!key.compare("ResourceGatherDuration")){
                settingsInt.at(BuildingInventoryUseCooldown) = std::stoi(value);
            }
		
            if(!key.compare("ResourceRand")){
                settingsInt.at(BuildingInventoryUseCooldown) = std::stoi(value);
            }
		
            if(!key.compare("ForestFrequency")){
                settingsInt.at(BuildingInventoryUseCooldown) = std::stoi(value);
            }
		
            if(!key.compare("ForestRand")){
                settingsInt.at(BuildingInventoryUseCooldown) = std::stoi(value);
            }
		
            if(!key.compare("ForestRoof")){
                settingsInt.at(BuildingInventoryUseCooldown) = std::stoi(value);
            }
		
            if(!key.compare("WoodPerTree")){
                settingsInt.at(BuildingInventoryUseCooldown) = std::stoi(value);
            }
		
            if(!key.compare("StoneFrequency")){
                settingsInt.at(BuildingInventoryUseCooldown) = std::stoi(value);
            }
		
            if(!key.compare("StoneRand")){
                settingsInt.at(BuildingInventoryUseCooldown) = std::stoi(value);
            }
		
            if(!key.compare("StoneRoof")){
                settingsInt.at(BuildingInventoryUseCooldown) = std::stoi(value);
            }
		
            if(!key.compare("StonePerBlock")){
                settingsInt.at(BuildingInventoryUseCooldown) = std::stoi(value);
            }
		
            if(!key.compare("TerrainFrequency")){
                settingsInt.at(BuildingInventoryUseCooldown) = std::stoi(value);
            }
        }
	}
	SettingsInt = settingsInt;
	return true;
}

int CSettingManager::GetVar(int varId){
    return SettingsInt.at(varId);
}

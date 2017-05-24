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
        }
	}
	SettingsInt = settingsInt;
	return true;
}

int CSettingManager::GetVar(int varId){
    return SettingsInt.at(varId);
}

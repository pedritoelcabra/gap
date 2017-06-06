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
                settingsInt.at(GathererBuildingStorage) = std::stoi(value);
            }

            if(!key.compare("ProductionBuildingInputStorage")){
                settingsInt.at(ProductionBuildingInputStorage) = std::stoi(value);
            }

            if(!key.compare("ProductionBuildingOutputStorage")){
                settingsInt.at(ProductionBuildingOutputStorage) = std::stoi(value);
            }

            if(!key.compare("BuildDuration")){
                settingsInt.at(BuildDuration) = std::stoi(value);
            }

            if(!key.compare("ResourceGatherDuration")){
                settingsInt.at(ResourceGatherDuration) = std::stoi(value);
            }

            if(!key.compare("ResourceRand")){
                settingsInt.at(ResourceRand) = std::stoi(value);
            }

            if(!key.compare("ForestFrequency")){
                settingsInt.at(ForestFrequency) = std::stoi(value);
            }

            if(!key.compare("ForestRand")){
                settingsInt.at(ForestRand) = std::stoi(value);
            }

            if(!key.compare("ForestRoof")){
                settingsInt.at(ForestRoof) = std::stoi(value);
            }

            if(!key.compare("WoodPerTree")){
                settingsInt.at(WoodPerTree) = std::stoi(value);
            }

            if(!key.compare("StoneFrequency")){
                settingsInt.at(StoneFrequency) = std::stoi(value);
            }

            if(!key.compare("StoneRand")){
                settingsInt.at(StoneRand) = std::stoi(value);
            }

            if(!key.compare("StoneRoof")){
                settingsInt.at(StoneRoof) = std::stoi(value);
            }

            if(!key.compare("StonePerBlock")){
                settingsInt.at(StonePerBlock) = std::stoi(value);
            }

            if(!key.compare("TerrainFrequency")){
                settingsInt.at(TerrainFrequency) = std::stoi(value);
            }

            if(!key.compare("CopperFrequency")){
                settingsInt.at(CopperFrequency) = std::stoi(value);
            }

            if(!key.compare("CopperRand")){
                settingsInt.at(CopperRand) = std::stoi(value);
            }

            if(!key.compare("CopperRoof")){
                settingsInt.at(CopperRoof) = std::stoi(value);
            }

            if(!key.compare("CopperPerBlock")){
                settingsInt.at(CopperPerBlock) = std::stoi(value);
            }

            if(!key.compare("TinFrequency")){
                settingsInt.at(TinFrequency) = std::stoi(value);
            }

            if(!key.compare("TinRand")){
                settingsInt.at(TinRand) = std::stoi(value);
            }

            if(!key.compare("TinRoof")){
                settingsInt.at(TinRoof) = std::stoi(value);
            }

            if(!key.compare("TinPerBlock")){
                settingsInt.at(TinPerBlock) = std::stoi(value);
            }

            if(!key.compare("CoalFrequency")){
                settingsInt.at(CoalFrequency) = std::stoi(value);
            }

            if(!key.compare("CoalRand")){
                settingsInt.at(CoalRand) = std::stoi(value);
            }

            if(!key.compare("CoalRoof")){
                settingsInt.at(CoalRoof) = std::stoi(value);
            }

            if(!key.compare("CoalPerBlock")){
                settingsInt.at(CoalPerBlock) = std::stoi(value);
            }

            if(!key.compare("GoldFrequency")){
                settingsInt.at(GoldFrequency) = std::stoi(value);
            }

            if(!key.compare("GoldRand")){
                settingsInt.at(GoldRand) = std::stoi(value);
            }

            if(!key.compare("GoldRoof")){
                settingsInt.at(GoldRoof) = std::stoi(value);
            }

            if(!key.compare("GoldPerBlock")){
                settingsInt.at(GoldPerBlock) = std::stoi(value);
            }

            if(!key.compare("SilverFrequency")){
                settingsInt.at(SilverFrequency) = std::stoi(value);
            }

            if(!key.compare("SilverRand")){
                settingsInt.at(SilverRand) = std::stoi(value);
            }

            if(!key.compare("SilverRoof")){
                settingsInt.at(SilverRoof) = std::stoi(value);
            }

            if(!key.compare("SilverPerBlock")){
                settingsInt.at(SilverPerBlock) = std::stoi(value);
            }

            if(!key.compare("IronFrequency")){
                settingsInt.at(IronFrequency) = std::stoi(value);
            }

            if(!key.compare("IronRand")){
                settingsInt.at(IronRand) = std::stoi(value);
            }

            if(!key.compare("IronRoof")){
                settingsInt.at(IronRoof) = std::stoi(value);
            }

            if(!key.compare("IronPerBlock")){
                settingsInt.at(IronPerBlock) = std::stoi(value);
            }

            if(!key.compare("LapisFrequency")){
                settingsInt.at(LapisFrequency) = std::stoi(value);
            }

            if(!key.compare("LapisRand")){
                settingsInt.at(LapisRand) = std::stoi(value);
            }

            if(!key.compare("LapisRoof")){
                settingsInt.at(LapisRoof) = std::stoi(value);
            }

            if(!key.compare("LapisPerBlock")){
                settingsInt.at(LapisPerBlock) = std::stoi(value);
            }

            if(!key.compare("GemFrequency")){
                settingsInt.at(GemFrequency) = std::stoi(value);
            }

            if(!key.compare("GemRand")){
                settingsInt.at(GemRand) = std::stoi(value);
            }

            if(!key.compare("GemRoof")){
                settingsInt.at(GemRoof) = std::stoi(value);
            }

            if(!key.compare("GemPerBlock")){
                settingsInt.at(GemPerBlock) = std::stoi(value);
            }

            if(!key.compare("EbonyFrequency")){
                settingsInt.at(EbonyFrequency) = std::stoi(value);
            }

            if(!key.compare("EbonyRand")){
                settingsInt.at(EbonyRand) = std::stoi(value);
            }

            if(!key.compare("EbonyRoof")){
                settingsInt.at(EbonyRoof) = std::stoi(value);
            }

            if(!key.compare("EbonyPerBlock")){
                settingsInt.at(EbonyPerBlock) = std::stoi(value);
            }

            if(!key.compare("LimeFrequency")){
                settingsInt.at(LimeFrequency) = std::stoi(value);
            }

            if(!key.compare("LimeFrequency")){
                settingsInt.at(LimeFrequency) = std::stoi(value);
            }

            if(!key.compare("LimeRoof")){
                settingsInt.at(LimeRoof) = std::stoi(value);
            }

            if(!key.compare("LimePerBlock")){
                settingsInt.at(LimePerBlock) = std::stoi(value);
            }

            if(!key.compare("MarbleFrequency")){
                settingsInt.at(MarbleFrequency) = std::stoi(value);
            }

            if(!key.compare("MarbleRand")){
                settingsInt.at(MarbleRand) = std::stoi(value);
            }

            if(!key.compare("MarbleRoof")){
                settingsInt.at(MarbleRoof) = std::stoi(value);
            }

            if(!key.compare("MarblePerBlock")){
                settingsInt.at(MarblePerBlock) = std::stoi(value);
            }

            if(!key.compare("AllTechsResearched")){
                settingsInt.at(AllTechsResearched) = std::stoi(value);
            }

            if(!key.compare("MineralLevel")){
                settingsInt.at(MineralLevel) = std::stoi(value);
            }

            if(!key.compare("WaterLevel")){
                settingsInt.at(WaterLevel) = std::stoi(value);
            }

            if(!key.compare("BeachLevel")){
                settingsInt.at(BeachLevel) = std::stoi(value);
            }

            if(!key.compare("GrassLevel")){
                settingsInt.at(GrassLevel) = std::stoi(value);
            }

            if(!key.compare("DryLandLevel")){
                settingsInt.at(DryLandLevel) = std::stoi(value);
            }

            if(!key.compare("MaxTownConnectionRangeFactor")){
                settingsInt.at(MaxTownConnectionRangeFactor) = std::stoi(value);
            }

            if(!key.compare("ResearchSpeed")){
                settingsInt.at(ResearchSpeed) = std::stoi(value);
            }
        }
	}
	SettingsInt = settingsInt;
	return true;
}

int CSettingManager::GetVar(int varId){
    return SettingsInt.at(varId);
}

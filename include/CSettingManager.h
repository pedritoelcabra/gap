#ifndef CSETTINGMANAGER_H
#define CSETTINGMANAGER_H

#include <vector>
#include <dirent.h>
#include <cstring>
#include <sstream>
#include <string>
#include <fstream>

class CSettingManager
{
    public:
        enum ESettings { ChunkGenerationDistance, BuildingInventoryUseCooldown, GathererBuildingStorage,
                       ProductionBuildingInputStorage, ProductionBuildingOutputStorage, BuildDuration,
                       ResourceGatherDuration, ResourceRand, ForestFrequency, ForestRand, ForestRoof,
                       WoodPerTree, StoneFrequency, StoneRand, StoneRoof, StonePerBlock, TerrainFrequency };

        CSettingManager(){};
        virtual ~CSettingManager(){};

        bool                                Init();
        int                                 GetVar(int varId);

    private:

        std::vector<int>                    SettingsInt;
};

#endif // CSETTINGMANAGER_H

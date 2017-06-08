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
                       WoodPerTree, StoneFrequency, StoneRand, StoneRoof, StonePerBlock, TerrainFrequency,
                       CopperFrequency,CopperRand,CopperRoof,CopperPerBlock,TinFrequency,TinRand,TinRoof,
                       TinPerBlock,CoalFrequency,CoalRand,CoalRoof,CoalPerBlock,GoldFrequency,GoldRand,GoldRoof,
                       GoldPerBlock,SilverFrequency,SilverRand,SilverRoof,SilverPerBlock,IronFrequency,IronRand,
                       IronRoof,IronPerBlock,LapisFrequency,LapisRand,LapisRoof,LapisPerBlock,GemFrequency,
                       GemRand,GemRoof,GemPerBlock,EbonyFrequency,EbonyRand,EbonyRoof,EbonyPerBlock,LimeFrequency,
                       LimeRand,LimeRoof,LimePerBlock,MarbleFrequency,MarbleRand,MarbleRoof,MarblePerBlock,
                       AllTechsResearched, WaterLevel, BeachLevel, GrassLevel, MineralLevel, DryLandLevel,
                       MaxTownConnectionRangeFactor, ResearchSpeed, BuildSpeed };

        CSettingManager(){};
        virtual ~CSettingManager(){};

        bool                                Init();
        int                                 GetVar(int varId);

    private:

        std::vector<int>                    SettingsInt;
};

#endif // CSETTINGMANAGER_H

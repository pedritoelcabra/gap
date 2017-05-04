#ifndef CBUILDINGMANAGER_H
#define CBUILDINGMANAGER_H

#include <vector>
#include <dirent.h>
#include <memory>
#include "CBuildingType.h"
#include "CBuilding.h"

typedef std::shared_ptr<CBuilding> build_shared_ptr;
typedef std::weak_ptr<CBuilding> build_weak_ptr;

class CBuildingManager
{
    public:
        CBuildingManager(){};
        virtual ~CBuildingManager(){};

        bool                                Init();
        void                                Update();
        void                                UpdateConnections();
        build_weak_ptr                      AddBuilding(int type, int x, int y, int owner);
        void                                Render();
        CBuildingType*                      GetBuildingType(int type);
        std::vector<CBuildingType>          GetBuildingTypes();
        void                                DemolishBuilding(int x, int y);
        build_weak_ptr                      GetBuildingAt(int x, int y);
        build_weak_ptr                      GetBuilding(int id);
        build_weak_ptr                      FindCollision(int x, int y);
        std::vector<build_weak_ptr>         GetUnfinishedBuildings( int radius, int x, int y);
    protected:

    private:
        void                                LoadBuildingType(std::string fileName);
        std::vector<CBuildingType>          BuildingTypes;
        std::vector<build_shared_ptr>       Buildings;
        int                                 buildingCount = 0;
};

#endif // CBUILDINGMANAGER_H

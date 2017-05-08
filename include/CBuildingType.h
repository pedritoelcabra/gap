#ifndef CBUILDINGTYPE_H
#define CBUILDINGTYPE_H

#include <cstring>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <SDL2/SDL.h>
#include "SDL2/SDL_gpu.h"
#include "CScreen.h"
#include "CAction.h"
#include "CGood.h"

class CBuildingType
{
    public:
        CBuildingType();
        virtual ~CBuildingType();

        void                                LoadLine(std::string key, std::string value);
        int                                 GetCost(int type);

        std::vector< std::vector<float> >   GetLayout(){            return Layout;};
        std::map< int, int >                GetBuildCosts(){        return BuildCosts;};
        std::map< int, int >                GetInput(){             return Input;};
        std::map< int, int >                GetOutput(){            return Output;};
        int                                 GetW(){                 return w;};
        int                                 GetH(){                 return h;};
        std::string                         GetName(){              return name;};
        std::string                         GetDescription(){       return description;};
        GPU_Rect                            GetClip(){              return clip;};
        vec2i                               GetDoor(){              return door;};
        int                                 MaxPop(){               return popMax;};
        int                                 WorkerCount(){          return workerCount;};
        int                                 WorkerPriority(){       return workerPriority;};
        int                                 PopCost(){              return popCost;};
        int                                 PopRange(){             return popRange;};
        int                                 BuildArea(){            return buildArea;};
        int                                 ResourceArea(){         return resourceRadius;};
        int                                 DistributionRange(){    return distributionRange;};
        int                                 TransportRange(){       return transportRange;};
        int                                 GetResource(){          return resource;};
        int                                 GetStorage(){           return maxStorage;};
        bool                                IsDraggable(){          return draggable;};
        bool                                IsRoad(){               return connects;};


        int                                 BuildCost(int res_);
        int                                 ConsumesResource(int res_ = 0);
        int                                 ProducesResource(int res_ = 0);
    protected:

    private:
        std::string                             name;
        std::string                             description;
        std::string                             upgrade;
        std::vector<std::string>                Requirements;
        std::vector< std::vector<float> >       Layout;
        std::map< int, int >                    BuildCosts;
        std::vector<CRecipe*>                   Recipes;
        int                                     w = -1;
        int                                     h = -1;
        bool                                    draggable = false;
        bool                                    connects = false;
        GPU_Rect                                clip;
        vec2i                                   door;
        int                                     resource = 0;
        int                                     resourceRadius = 0;
        int                                     buildArea = 0;
        int                                     distributionRange = 0;
        int                                     transportRange = 0;

        int                                     popMax = 0;
        int                                     popCost = 0;
        int                                     popRange = 0;
        int                                     workerCount = 0;
        int                                     workerPriority = 0;
        int                                     maxStorage = 0;

        void            LoadCosts(std::map< int, int > & container, std::string value);
        void            LoadLayout(std::string value);
        void            InitLayout();
};

#endif // CBUILDINGTYPE_H

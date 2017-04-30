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

class CBuildingType
{
    public:
        CBuildingType();
        virtual ~CBuildingType();

        void                                LoadLine(std::string key, std::string value);
        std::vector< std::vector<float> >   GetLayout();
        int                                 GetW();
        int                                 GetH();
        std::string                         GetName();
        std::string                         GetDescription();
        GPU_Rect                            GetClip();
        vec2i                               getDoor();
        int                                 MaxPop();
        int                                 WorkerCount();
        int                                 WorkerPriority();
        int                                 PopCost();
        int                                 PopRange();
        int                                 BuildArea();
        int                                 ResourceArea();
        int                                 DistributionRange();
        int                                 getResource();
        bool                                isDraggable();
        int                                 GetCost(int type);
    protected:

    private:
        std::string                             name;
        std::string                             description;
        std::string                             upgrade;
        std::vector<std::string>                Requirements;
        std::vector< std::vector<float> >       Layout;
        std::map< int, int >                    BuildCosts;
        int                                     w;
        int                                     h;
        bool                                    draggable;
        bool                                    connects;
        GPU_Rect                                clip;
        vec2i                                   door;
        int                                     resource;
        int                                     resourceRadius;
        int                                     buildArea;
        int                                     distributionRange;

        int                                     popMax;
        int                                     popCost;
        int                                     popRange;
        int                                     workerCount;
        int                                     workerPriority;

        void            LoadCosts(std::string value);
        void            LoadLayout(std::string value);
        void            InitLayout();
};

#endif // CBUILDINGTYPE_H

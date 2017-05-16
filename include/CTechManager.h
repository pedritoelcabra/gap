#ifndef CTECHMANAGER_H
#define CTECHMANAGER_H

#include <vector>
#include <dirent.h>
#include "CTech.h"

class CTechManager
{
    public:
        CTechManager(){};
        virtual ~CTechManager(){};

        bool                                Init();
        Tech*                               GetTechByName(const std::string * name){    return Techs.at(TechsByName.at(*name)); };
        bool                                IsAvailable(int tech){                      return Techs.at(tech).IsResearched(); };
        bool                                IsResearched(int tech){                     return Techs.at(tech).IsAvailable(); };
        bool                                AddProgress(int tech);
    protected:

    private:
        int                                 techCounter = 0;
        void                                LoadTech(std::string fileName);
        std::map<int, CTech>                Techs;
        std::map<std::string, int>          TechsByName;
};

#endif // CTECHMANAGER_H

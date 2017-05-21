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
        CTech*                              GetTechByName(const std::string * name){    return &Techs.at(TechsByName.at(*name)); };
        CTech*                              GetTechById(int id){                        return &Techs.at(id); };
        std::map<int, CTech>*               GetTechs(){                                 return &Techs; };
        bool                                IsAvailable(int tech){                      return Techs.at(tech).IsAvailable(); };
        bool                                IsResearched(int tech){                     return Techs.at(tech).IsResearched(); };
        bool                                IsResearching(){                            return isResearching; };
        bool                                AddProgress(int researchLvl);
        void                                StartResearch(int tech);
        CTech*                              CurrentTech(){                              return &Techs.at(currentTech); };
        int                                 CurrentTechLvL(){                           return Techs.at(currentTech).GetLvL(); };
        std::string*                        CurrentTechName();

    protected:

    private:
        int                                 techCounter = 0;
        void                                LoadTech(std::string fileName);
        std::map<int, CTech>                Techs;
        std::map<std::string, int>          TechsByName;
        int                                 currentTech = 0;
        int                                 currentTechLvL = 0;
        bool                                isResearching = false;
        std::string                         noTechName;
};

#endif // CTECHMANAGER_H

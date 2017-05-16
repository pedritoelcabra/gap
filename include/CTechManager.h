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
        Tech*                               GetTechByName(const std::string * name);
        bool                                IsAvailable(int tech);
        bool                                IsResearched(int tech);
        bool                                AddProgress(int tech);
    protected:

    private:
        void                                LoadTech(std::string fileName);
        std::map<std::string, CTech>        Techs;
};

#endif // CTECHMANAGER_H

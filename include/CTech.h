#ifndef CTECH_H
#define CTECH_H

#include <cstring>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include "CGood.h"

class CTech
{
    public:
        CTech(){};
        virtual ~CTech(){};

        void                                    LoadLine(std::string key, std::string value);
        bool                                    IsAvailable(){      return isAvailable; };
        bool                                    IsResearched(){     return isResearched; };
        bool                                    AddProgress();
        void                                    Init();
        void                                    SetId(int id_){      id = id_; };
        int                                     GetId(){             return id; };
        int                                     GetProgress(){             return currentProgress; };
        int                                     GetCost(){             return techCost; };
        int                                     GetLvL(){             return minSkill; };
        void                                    UpdateStatus();
        std::string*                            GetName(){              return &name;};

    protected:

    private:
        std::string                             name;
        std::string                             description;
        std::map< int, int >                    Input;
        std::vector< int >                      Requirements;
        std::vector< std::string >              RequirementsNames;
        bool                                    isAvailable = false;
        bool                                    isResearched = false;
        int                                     techCost = 1;
        int                                     currentProgress = 0;
        int                                     minSkill = 1;
        int                                     id;

        void                                    LoadCosts(std::map< int, int > & container, std::string value);
};

#endif // CTECH_H

#ifndef CTECH_H
#define CTECH_H

#include <cstring>
#include <sstream>
#include <string>
#include <map>
#include <vector>

class CTech
{
    public:
        CTech(){};
        virtual ~CTech(){};

        void                                    LoadLine(std::string key, std::string value);
    protected:

    private:
        std::string                             name;
        std::map< int, int >                    Input;
        std::vector< int >                      Requirements;
        std::vector< std::string >              RequirementsNames;
        bool                                    isAvailable = false;
        bool                                    isResearched = false;
        int                                     techCost = 1;
        int                                     currentProgress = 0;
        int                                     minSkill = 0;

        void                                    LoadCosts(std::map< int, int > & container, std::string value);
};

#endif // CTECH_H

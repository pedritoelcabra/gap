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
        CTech();
        virtual ~CTech();

        void                                    LoadLine(std::string key, std::string value);
    protected:

    private:
        std::string                             name;
        std::map< int  >                        Requirements;
        bool                                    isAvailable = false;
        bool                                    isResearched = false;
        int                                     totalCost = 1;
        int                                     currentProgress = 0;
        int                                     requiredSkill = 0;

        void                                    LoadCosts(std::map< int, int > & container, std::string value);
};

#endif // CTECH_H

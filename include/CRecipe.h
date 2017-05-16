#ifndef CRECIPE_H
#define CRECIPE_H

#include <cstring>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include "CGood.h"

class CRecipe
{
    public:
        CRecipe(){};
        virtual ~CRecipe(){};

        void                                LoadLine(std::string key, std::string value);
        std::map< int, int >*               GetInput(){             return &Input;};
        std::map< int, int >*               GetOutput(){            return &Output;};
        std::string*                        GetName(){              return &name;};
        int                                 ConsumesResource(int res_);
        int                                 ProducesResource(int res_);
    protected:

    private:
        std::string                             name;
        std::map< int, int >                    Input;
        std::map< int, int >                    Output;

        void            LoadCosts(std::map< int, int > & container, std::string value);
};

#endif // CRECIPE_H

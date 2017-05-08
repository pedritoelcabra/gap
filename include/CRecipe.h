#ifndef CRECIPE_H
#define CRECIPE_H

#include <cstring>
#include <sstream>
#include <string>
#include <map>
#include <vector>

class CRecipe
{
    public:
        CRecipe();
        virtual ~CRecipe();

        void                                LoadLine(std::string key, std::string value);
    protected:

    private:
        std::map< int, int >                    Input;
        std::map< int, int >                    Output;

        void            LoadCosts(std::map< int, int > & container, std::string value);
};

#endif // CRECIPE_H

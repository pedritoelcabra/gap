#ifndef CRECIPEMANAGER_H
#define CRECIPEMANAGER_H

#include <vector>
#include <dirent.h>
#include "CRecipe.h"

class CRecipeManager
{
    public:
        CRecipeManager(){};
        virtual ~CRecipeManager(){};

        bool                                Init();
        CRecipe                             GetRecipeByName(std::string name);
    protected:

    private:
        void                                LoadRecipe(std::string fileName);
        std::map<std::string, CRecipe>      Recipes;
};

#endif // CRECIPEMANAGER_H

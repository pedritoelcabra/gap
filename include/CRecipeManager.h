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
    protected:

    private:
        void                                LoadRecipe(std::string fileName);
        std::vector<CRecipe>                Recipes;
};

#endif // CRECIPEMANAGER_H

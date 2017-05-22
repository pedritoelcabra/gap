#include "CRecipeManager.h"
#include "CGame.h"

extern CGame GAP;

bool CRecipeManager::Init(){
    CLog::Write("Loading Recipes");
    DIR* dir = opendir("defines/recipes");
    if (!dir){
        CLog::Write("Couldnt open recipe directory");
        return false;
    }

    struct dirent* entry;
    std::string fileName = "";
    while( (entry = readdir(dir)) != NULL ){
        if(entry->d_namlen < 3) continue;
        fileName = "defines/recipes/";
        fileName.append(entry->d_name);
        LoadRecipe(fileName);
    }
    return true;
}

void CRecipeManager::LoadRecipe(std::string fileName){
  CLog::Write(fileName);

	CRecipe recipe = CRecipe ();
	std::ifstream is_file(fileName);
	std::string line = "";

    if (is_file.is_open()){
        while( std::getline(is_file, line) ) {
            std::istringstream is_line(line);
            std::string key = "";
            std::string value = "";
            std::getline(is_line, key, '=');
            std::getline(is_line, value);
            recipe.LoadLine(key, value);
        }
	}
    recipe.Init();
	Recipes[*(recipe.GetName())] = recipe;
}

CRecipe CRecipeManager::GetRecipeByName(std::string name){
	return Recipes.at(name);
}

void CRecipeManager::UpdateAvailabilities(){
    for(auto & recipe : Recipes){
        recipe.second.UpdateAvailability();
    }
}

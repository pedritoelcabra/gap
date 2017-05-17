#include "CTechManager.h"
#include "CGame.h"

extern CGame GAP;

bool CTechManager::Init(){
    CLog::Write("Loading Techs");
    DIR* dir = opendir("defines/techs");
    if (!dir){
        CLog::Write("Couldnt open tech directory");
        return false;
    }

    struct dirent* entry;
    std::string fileName = "";
    while( (entry = readdir(dir)) != NULL ){
        if(entry->d_namlen < 3) continue;
        fileName = "defines/techs/";
        fileName.append(entry->d_name);
        LoadTech(fileName);
    }
    return true;
}

void CTechManager::LoadTech(std::string fileName){
  	CLog::Write(fileName);

	CTech tech = CTech ();
	std::ifstream is_file(fileName);
	std::string line = "";

    if (is_file.is_open()){
        while( std::getline(is_file, line) ) {
            std::istringstream is_line(line);
            std::string key = "";
            std::string value = "";
            std::getline(is_line, key, '=');
            std::getline(is_line, value);
            tech.LoadLine(key, value);
        }
	}
	Techs[techCounter] = tech;
	TechsByName[*(tech.GetName())] = techCounter;
	techCounter++;
}

bool CTechManager::AddProgress(int tech){
    return Techs[tech].AddProgress();
}

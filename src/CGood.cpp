#include "CGood.h"

std::map<int, std::string > CGood::ResourceNames = {
   {CGood::wood, "Wood"},
   {CGood::stone, "Stone"},
   {CGood::work, "Work"},
   {CGood::plank, "Plank"},
   {CGood::cereal, "Cereal"}
};

std::map<int, int > CGood::ResourceIcons = {
   {CGood::wood, 3051},
   {CGood::stone, 2996},
   {CGood::work, 1},
   {CGood::plank, 3052},
   {CGood::cereal, 3053}
};

const std::map<int, std::string > CGood::GetResources(){
    return ResourceNames;
}

const std::string CGood::GetResourceName(int resource){
    return ResourceNames.at(resource);
}

const int CGood::GetResourceByName(std::string resourceName){
    for (auto it = ResourceNames.begin(); it != ResourceNames.end(); ++it ){
        if (!resourceName.compare(it->second)){
            return it->first;
        }
    }
    return 0;
}

const int CGood::GetResourceIcon(int resource){
    return ResourceIcons.at(resource);
}


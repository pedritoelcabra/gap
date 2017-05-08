#include "CGood.h"

std::map<int, std::string > CGood::ResourceNames = {
   {1, "Wood"},
   {2, "Stone"},
   {3, "Work"},
   {4, "Plank"},
   {5, "Cereal"}
};

std::map<int, int > CGood::ResourceIcons = {
   {1, 3051},
   {2, 2996},
   {3, 1},
   {4, 3052},
   {5, 3053}
};

const std::map<int, std::string > CGood::GetResources(){
    return ResourceNames;
}

const std::string CGood::GetResourceName(int resource){
    return ResourceNames.at(resource);
}

const int CGood::GetResourceByName(std::string resourceName){
    for (auto it = ResourceNames.begin(); it != ResourceNames.end(); ++it ){
        if (resourceName.compare(it->second)){
            return it->first;
        }
    }
    return 0;
}

const int CGood::GetResourceIcon(int resource){
    return ResourceIcons.at(resource);
}


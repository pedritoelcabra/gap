#include "CGood.h"

std::map<int, std::string > CGood::Resources = {
   {1, "Wood"},
   {2, "Stone"},
   {3, "Work"}
};


const std::map<int, std::string > CGood::GetResources(){
    return Resources;
}

const std::string CGood::GetResourceName(int resource){
    return Resources.at(resource);
}

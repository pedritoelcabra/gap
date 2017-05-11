#ifndef CGOOD_H
#define CGOOD_H

#include <map>

class CGood
{
    public:
        enum EResources { nothing, wood, stone, work, plank, cereal };

        static const std::map<int, std::string >    GetResources();
        static const std::string                    GetResourceName(int resource);
        static const int                            GetResourceByName(std::string resourceName);
        static const int                            GetResourceIcon(int resource);

    protected:

    private:
        static std::map<int, std::string >          ResourceNames;
        static std::map<int, int >                  ResourceIcons;
};

#endif // CGOOD_H

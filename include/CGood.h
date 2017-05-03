#ifndef CGOOD_H
#define CGOOD_H

#include <map>

class CGood
{
    public:
        enum EResources { nothing, wood, stone, work };

        static const std::map<int, std::string >    GetResources();
        static const std::string                    GetResourceName(int resource);

    protected:

    private:
        static std::map<int, std::string >          Resources;
};

#endif // CGOOD_H

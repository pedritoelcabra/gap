#ifndef CENEMY_H
#define CENEMY_H

#include <dirent.h>
#include <cstring>
#include <sstream>
#include <string>
#include <fstream>
#include <CUnit.h>


class CEnemy : public CUnit
{
    public:
        CEnemy(){};
        CEnemy(int x_, int y_, std::string myName);
        virtual ~CEnemy(){};

        void                                LoadLine(std::string key, std::string value);
        void                                UpdateAssignment();
    protected:

    private:
};

#endif // CENEMY_H

#ifndef CTREE_H
#define CTREE_H

#include <CGuiObject.h>


class CTree : public CGUIObject
{
    public:
        CTree();
        virtual ~CTree();
        void            Set(int x_, int y_, int type_);
        bool            Render();
    protected:

    private:
        int                 type;
};

#endif // CTREE_H

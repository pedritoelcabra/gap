#ifndef CTREE_H
#define CTREE_H

#include <CGuiObject.h>


class CTree : public CGUIObject
{
    public:
        CTree();
        virtual ~CTree();
        void            Set(int x_, int y_, int type_, int variety_);
        bool            Render(bool absolute = false);
    protected:

    private:
        int                 type;
        int                 variety;
};

#endif // CTREE_H

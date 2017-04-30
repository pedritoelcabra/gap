/// PPP - Menu header

#ifndef _CMENU_H
#define _CMENU_H

#include <vector>
#include <memory>
#include "CButton.h"
#include "CLog.h"
#include "CTile.h"
#include "CCharacter.h"
#include "CGuiObject.h"

class CGUIObject;

class CMenu{
public:
    CMenu();
    virtual ~CMenu();

    virtual void                    Render();
    int                             ButtonCount() const;
    bool                            FindCollision(int x, int y);
    bool                            HandleLClick(int x, int y);
    virtual void                    Clicked(CButton button){ std::terminate(); };
protected:
    std::vector<CButton>  PopUpButtons;

};

#endif // _CMENU_H

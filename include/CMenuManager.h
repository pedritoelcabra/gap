#ifndef CMENUMANAGER_H
#define CMENUMANAGER_H

#include <CMenu.h>
#include "CContextMenu.h"
#include "CBuildingMenu.h"
#include "CIngameMenu.h"
#include "CInfoMenu.h"
#include "CGuiObject.h"
#include "CBuilding.h"
#include "CPathfinder.h"

class CGuiObject;
class CBuilding;

typedef std::shared_ptr<CBuilding> build_shared_ptr;
typedef std::weak_ptr<CBuilding> build_weak_ptr;

class CMenuManager
{
    public:
        CMenuManager();
        virtual ~CMenuManager();

        bool                        HandleRClick(int x, int y);
        bool                        HandleLClick(int x, int y);
        void                        BuildMenus();
        void                        ShowBuildingMenu();
        void                        HideBuildingMenus();
        void                        HideContextMenus();
        void                        Render();
        void                        MousePointerBuilding(int type);
        void                        MousePointerDemolition();
        void                        SetHighLightCirce(build_weak_ptr ptr);
        void                        ShowInfoUnit(unit_weak_ptr ptr);
    protected:

    private:
        std::vector<CContextMenu>                   ContextMenus;
        std::vector<CIngameMenu>                    IngameMenus;
        std::vector<CBuildingMenu>                  BuildingMenus;
        std::vector<CInfoMenu>                      InfoMenus;

        bool                                        contextOpen;
        bool                                        buildingOpen;

        build_shared_ptr                            mouseBuilding;
        bool                                        mouseHasBuilding;
        bool                                        mouseIsDemolition;
        bool                                        mouseHasDraggable;
        bool                                        draggedPlanted;
        int                                         dragStartX;
        int                                         dragStartY;
        int                                         dragEndX;
        int                                         dragEndY;
        std::vector<Coord>                          dragPath;

        int                                         highLightX;
        int                                         highLightY;
        int                                         highLightRadius;
        SDL_Color                                   highLightColor;
};

#endif // CMENUMANAGER_H

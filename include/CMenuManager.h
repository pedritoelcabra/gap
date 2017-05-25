#ifndef CMENUMANAGER_H
#define CMENUMANAGER_H

#include <CMenu.h>
#include "CContextMenu.h"
#include "CBuildingMenu.h"
#include "CIngameMenu.h"
#include "CInfoMenu.h"
#include "CTechMenu.h"
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
        CMenuManager(){};
        virtual ~CMenuManager(){};

        bool                        HandleRClick(int x, int y);
        bool                        HandleLClick(int x, int y);
        bool                        HandleLClickUp(int x, int y);
        bool                        OnMouseWheel(bool Up, int x, int y);
        bool                        HandleMouseMovement(int x, int y);
        void                        BuildMenus();
        void                        ShowBuildingMenu();
        void                        ShowTechMenu();
        void                        HideBuildingMenus();
        void                        HideTechMenus();
        void                        HideContextMenus();
        void                        Render();
        void                        MousePointerBuilding(int type);
        void                        MousePointerDemolition();
        void                        SetHighLightCirce(build_weak_ptr ptr);
        void                        ShowInfoUnit(unit_weak_ptr ptr);
        void                        ShowInfoBuilding(build_weak_ptr ptr);
        void                        ShowInfoTile(tile_weak_ptr ptr);
    protected:

    private:
        std::vector<CContextMenu>                   ContextMenus;
        std::vector<CIngameMenu>                    IngameMenus;
        std::vector<CBuildingMenu>                  BuildingMenus;
        std::vector<CInfoMenu>                      InfoMenus;
        std::vector<CTechMenu>                      TechMenus;

        bool                                        contextOpen = false;
        bool                                        buildingOpen = false;
        bool                                        techOpen = false;
        bool                                        showingTile = false;

        build_shared_ptr                            mouseBuilding;
        bool                                        mouseHasBuilding = false;
        bool                                        mouseIsDemolition = false;
        bool                                        mouseHasDraggable = false;
        bool                                        draggedPlanted = false;
        int                                         dragStartX = 0;
        int                                         dragStartY = 0;
        int                                         dragEndX = 0;
        int                                         dragEndY = 0;
        std::vector<Coord>                          dragPath;

        int                                         highLightX = 0;
        int                                         highLightY = 0;
        int                                         highLightRadius = 0;
        int                                         highLightRadius2 = 0;
        SDL_Color                                   highLightColor;
        SDL_Color                                   highLightColor2;
};

#endif // CMENUMANAGER_H

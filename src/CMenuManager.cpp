#include "CMenuManager.h"
#include "CGame.h"

extern CGame GAP;

CMenuManager::CMenuManager(){
    mouseHasBuilding = false;
    highLightX = 0;
    highLightY = 0;
    highLightRadius = 0;
}

CMenuManager::~CMenuManager(){
    //dtor
}

bool CMenuManager::HandleRClick(int x, int y){

    mouseHasBuilding = false;
    mouseIsDemolition = false;
    mouseHasDraggable = false;
    draggedPlanted = false;
    highLightRadius = 0;
    HideContextMenus();
    HideBuildingMenus();
    InfoMenus.clear();

    CContextMenu contextMenu = CContextMenu( x, y);

    if(contextMenu.ButtonCount() > 0){
        ContextMenus.push_back(contextMenu);
        contextOpen = true;
    }

    return true;
}

bool CMenuManager::HandleLClick(int x, int y){


    for(CIngameMenu& e : IngameMenus)    {
        if(e.HandleLClick( x,  y)){
            mouseHasBuilding = false;
            mouseIsDemolition = false;
            mouseHasDraggable = false;
            draggedPlanted = false;
            highLightRadius = 0;
            return true;
        }
    }

    if(mouseIsDemolition){
        GAP.BuildingManager.DemolishBuilding(GAP.getMouseTileX(), GAP.getMouseTileY());
        return true;
    }

    if(mouseHasDraggable){
        if(draggedPlanted){
            if(dragPath.size()){
                for(auto b : dragPath){
                    GAP.BuildingManager.AddBuilding(mouseBuilding->getType(),b.x,b.y,1);
                    dragStartX = dragEndX;
                    dragStartY = dragEndY;
                }
            }
        }else{
            draggedPlanted = true;
            dragStartX = dragEndX = GAP.getMouseTileX();
            dragStartY = dragEndY = GAP.getMouseTileY();
        }
        return true;
    }

    if(mouseHasBuilding){
        GAP.BuildingManager.AddBuilding(mouseBuilding->getType(),mouseBuilding->getTileX(),mouseBuilding->getTileY(),1);
        return true;
    }

    for(CContextMenu& e : ContextMenus)    {
        if(e.HandleLClick( x,  y)){
            return true;
        }
    }

    for(CBuildingMenu& e : BuildingMenus)    {
        if(e.HandleLClick( x,  y)){
            return true;
        }
    }

    auto w = GAP.BuildingManager.FindCollision(GAP.TranslateXStoW(x) , GAP.TranslateYStoW(y));
    if(w.lock()){
        SetHighLightCirce(w);
        InfoMenus.clear();
        CInfoMenu infoMenu = CInfoMenu(w);
        InfoMenus.push_back(infoMenu);
    }

    HideContextMenus();
    return false;
}

void CMenuManager::Render(){

    if(mouseHasBuilding){
        mouseBuilding->setX(GAP.getMouseTileX() - (mouseBuilding->GetTileWidth() / 2));
        mouseBuilding->setY(GAP.getMouseTileY() - (mouseBuilding->GetTileHeight() / 2));
        mouseBuilding->RenderOnTooltip();
        SetHighLightCirce(build_weak_ptr(mouseBuilding));
    }

    if(mouseHasDraggable){
        mouseBuilding->setX(GAP.getMouseTileX());
        mouseBuilding->setY(GAP.getMouseTileY());
        mouseBuilding->RenderOnTooltip();

        if(draggedPlanted){
            if(dragEndX != GAP.getMouseTileX() || dragEndY != GAP.getMouseTileY()){
                dragEndX = GAP.getMouseTileX();
                dragEndY = GAP.getMouseTileY();
                dragPath = GAP.Pathfinder.FindPath(
                      Coord(dragStartX, dragStartY),
                      Coord(GAP.getMouseTileX(), GAP.getMouseTileY()),
                      0.0f, 3.0f, false );
            }

            for(auto b : dragPath){
                mouseBuilding->setX(b.x);
                mouseBuilding->setY(b.y);
                mouseBuilding->RenderOnTooltip();
            }
        }
    }

    if(highLightRadius > 0){
        GAP.TextureManager.DrawHighLightCircle(highLightX, highLightY, highLightRadius, highLightColor);
    }

    for(CMenu& e : IngameMenus)    {
       e.Render();
    }
    for(CMenu& e : BuildingMenus)    {
       e.Render();
    }
    for(CMenu& e : ContextMenus)    {
       e.Render();
    }
    for(CMenu& e : InfoMenus)    {
       e.Render();
    }
}

void CMenuManager::SetHighLightCirce(build_weak_ptr ptr){

    if(auto s = ptr.lock()){
        highLightX = s->getDoor().first;
        highLightY = s->getDoor().second;

        if(s->DistributionRange()){
            highLightColor = {255, 255, 255, 50};
            highLightRadius = s->DistributionRange();
        }
        if(s->PopRange()){
            highLightColor = {255, 100, 100, 50};
            highLightRadius = s->PopRange();
        }
        if(s->BuildArea()){
            highLightColor = {100, 255, 100, 50};
            highLightRadius = s->BuildArea();
        }
        if(s->ResourceArea()){
            highLightColor = {100, 100, 255, 50};
            highLightRadius = s->ResourceArea();
        }
    }
}

void CMenuManager::BuildMenus(){
    CIngameMenu ingameMenu = CIngameMenu();
    IngameMenus.push_back(ingameMenu);
}

void CMenuManager::ShowBuildingMenu(){

    if(buildingOpen){
        HideBuildingMenus();
        return;
    }
    CBuildingMenu buildingMenu = CBuildingMenu();
    BuildingMenus.push_back(buildingMenu);
    buildingOpen = true;
}

void CMenuManager::HideBuildingMenus(){
    BuildingMenus.clear();
    buildingOpen = false;
}

void CMenuManager::HideContextMenus(){
    ContextMenus.clear();
    contextOpen = false;
}

void CMenuManager::MousePointerBuilding(int type){

    GAP.MenuManager.HideBuildingMenus();
    mouseBuilding = std::make_shared<CBuilding>(type, GAP.getMouseTileX(), GAP.getMouseTileY(), -1);
    if(GAP.BuildingManager.GetBuildingType(type)->isDraggable()){
        mouseHasDraggable = true;
        draggedPlanted = false;
        return;
    }
    mouseHasBuilding = true;
}

void CMenuManager::MousePointerDemolition(){
    GAP.MenuManager.HideBuildingMenus();
    mouseIsDemolition = true;
}

void CMenuManager::ShowInfoUnit(unit_weak_ptr ptr){
    InfoMenus.clear();
    CInfoMenu infoMenu = CInfoMenu(ptr);
    InfoMenus.push_back(infoMenu);
}

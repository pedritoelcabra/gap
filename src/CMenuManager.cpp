#include "CMenuManager.h"
#include "CGame.h"

extern CGame GAP;

bool CMenuManager::HandleRClick(int x, int y){

    mouseHasBuilding = false;
    mouseIsDemolition = false;
    mouseHasDraggable = false;
    draggedPlanted = false;
    highLightRadius = 0;
    highLightRadius2 = 0;
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
            return true;
        }
    }

    if(mouseIsDemolition){
        GAP.BuildingManager.DemolishBuilding(GAP.GetMouseTileX(), GAP.GetMouseTileY());
        return true;
    }

    if(mouseHasDraggable){
        if(draggedPlanted){
            if(dragPath.size()){
                GAP.BuildingManager.AddBuilding(mouseBuilding->GetType(),dragStartX,dragStartY,1);
                for(auto b : dragPath){
                    GAP.BuildingManager.AddBuilding(mouseBuilding->GetType(),b.x,b.y,1);
                    dragStartX = dragEndX;
                    dragStartY = dragEndY;
                }
            }
            draggedPlanted = false;
        }else{
            draggedPlanted = true;
            dragStartX = dragEndX = GAP.GetMouseTileX();
            dragStartY = dragEndY = GAP.GetMouseTileY();
        }
        return true;
    }

    if(mouseHasBuilding){
        GAP.BuildingManager.AddBuilding(mouseBuilding->GetType(),mouseBuilding->GetTileX(),mouseBuilding->GetTileY(),1);
        return true;
    }

    for(CContextMenu& e : ContextMenus)    {
        if(e.HandleLClick( x,  y)){
            return true;
        }
    }
    HideContextMenus();

    for(auto & e : BuildingMenus)    {
        if(e.HandleLClick( x,  y)){
            return true;
        }
    }

    for(auto & e : TechMenus)    {
        if(e.HandleLClick( x,  y)){
            return true;
        }
    }

    for(auto & e : InfoMenus)    {
        if(e.HandleLClick( x,  y)){
            return true;
        }
    }

    auto uv = GAP.UnitManager.FindCollision(GAP.TranslateXStoW(x) , GAP.TranslateYStoW(y));
    if(uv.size() > 0){
        if(auto u = uv.back().lock()){
            ShowInfoUnit(u);
            return true;
        }
    }

    auto w = GAP.BuildingManager.FindCollision(GAP.TranslateXStoW(x) , GAP.TranslateYStoW(y));
    if(w.lock()){
        ShowInfoBuilding(w);
        return true;
    }

    return false;
}

bool CMenuManager::HandleLClickUp(int x, int y){
    for(CMenu& e : InfoMenus)    {
       e.HandleLClickUp(x, y);
    }
    return true;
}

bool CMenuManager::HandleMouseMovement(int x, int y){

    for(CMenu& e : InfoMenus)    {
        if(e.HandleMouseMovement(x, y)){
            return true;
        }
    }
    ShowInfoTile(GAP.ChunkManager.GetTile(GAP.GetMouseTileX(), GAP.GetMouseTileY()));
    return false;
}

bool CMenuManager::OnMouseWheel(bool Up, int x, int y){
    for(CMenu& e : InfoMenus)    {
       if(e.OnMouseWheel(Up, x, y)){
            return true;
       }
    }
    return false;
}

void CMenuManager::Render(){

    if(mouseHasBuilding){
        if(auto s = GAP.BuildingManager.GetBuildingAt(GAP.GetMouseTileX(), GAP.GetMouseTileY()).lock()){
            mouseBuilding->SetX(s->GetTileX());
            mouseBuilding->SetY(s->GetTileY());
        }else{
            mouseBuilding->SetX(GAP.GetMouseTileX() - (mouseBuilding->GetTileWidth() / 2));
            mouseBuilding->SetY(GAP.GetMouseTileY() - (mouseBuilding->GetTileHeight() / 2));
        }
        mouseBuilding->RenderOnTooltip();
        SetHighLightCirce(build_weak_ptr(mouseBuilding));
    }

    if(mouseHasDraggable){
        mouseBuilding->SetX(GAP.GetMouseTileX());
        mouseBuilding->SetY(GAP.GetMouseTileY());
        mouseBuilding->RenderOnTooltip();

        if(draggedPlanted){
            if(dragEndX != GAP.GetMouseTileX() || dragEndY != GAP.GetMouseTileY()){
                dragEndX = GAP.GetMouseTileX();
                dragEndY = GAP.GetMouseTileY();
                dragPath = GAP.Pathfinder.FindPath(
                      Coord(dragStartX, dragStartY),
                      Coord(GAP.GetMouseTileX(), GAP.GetMouseTileY()),
                      0.0f, 3.0f, false );
            }

            mouseBuilding->SetX(dragStartX);
            mouseBuilding->SetY(dragStartY);
            mouseBuilding->RenderOnTooltip();
            for(auto b : dragPath){
                mouseBuilding->SetX(b.x);
                mouseBuilding->SetY(b.y);
                mouseBuilding->RenderOnTooltip();
            }
        }
    }

    if(highLightRadius2 > 0){
        GAP.TextureManager.DrawHighLightCircle(highLightX, highLightY, highLightRadius2, highLightColor2);
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
    for(CMenu& e : TechMenus)    {
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
    highLightRadius = 0;
    if(auto s = ptr.lock()){
        highLightX = s->GetDoor().first;
        highLightY = s->GetDoor().second;

        if(s->DistributionRange()){
            highLightColor = {255, 255, 255, 80};
            highLightRadius = s->DistributionRange();
        }
//        if(s->PopRange()){
//            highLightColor = {255, 100, 100, 50};
//            highLightRadius = s->PopRange();
//        }
//        if(s->BuildArea()){
//            highLightColor = {100, 255, 100, 50};
//            highLightRadius = s->BuildArea();
//        }
        if(s->ResourceArea()){
            highLightColor = {100, 100, 255, 50};
            highLightRadius = s->ResourceArea();
        }
//        if(s->TransportRange()){
//            highLightColor2 = {255, 255, 255, 30};
//            highLightRadius2 = s->TransportRange();
//        }
    }
}

void CMenuManager::BuildMenus(){
    IngameMenus.clear();
    CIngameMenu ingameMenu = CIngameMenu();
    IngameMenus.push_back(ingameMenu);
}

void CMenuManager::ShowBuildingMenu(){

    if(buildingOpen){
        HideBuildingMenus();
        return;
    }
    HideTechMenus();
    CBuildingMenu buildingMenu = CBuildingMenu();
    BuildingMenus.push_back(buildingMenu);
    buildingOpen = true;
}

void CMenuManager::ShowTechMenu(){
    if(techOpen){
        HideTechMenus();
        return;
    }
    HideBuildingMenus();
    CTechMenu techMenu = CTechMenu();
    TechMenus.push_back(techMenu);
    techOpen = true;
}

void CMenuManager::ClearMenus(){
    mouseHasBuilding = false;
    mouseIsDemolition = false;
    mouseHasDraggable = false;
    draggedPlanted = false;
    highLightRadius = 0;
    highLightRadius2 = 0;

}

void CMenuManager::HideBuildingMenus(){
    BuildingMenus.clear();
    buildingOpen = false;
}

void CMenuManager::HideTechMenus(){
    TechMenus.clear();
    techOpen = false;
}

void CMenuManager::HideContextMenus(){
    ContextMenus.clear();
    contextOpen = false;
}

void CMenuManager::MousePointerBuilding(int type){

    GAP.MenuManager.HideBuildingMenus();
    mouseBuilding = std::make_shared<CBuilding>(type, GAP.GetMouseTileX(), GAP.GetMouseTileY(), -1);
    if(GAP.BuildingManager.GetBuildingType(type)->IsDraggable()){
        mouseHasDraggable = true;
        draggedPlanted = false;
        return;
    }
    mouseHasBuilding = true;
    ShowInfoBuilding(build_weak_ptr(mouseBuilding));
}

void CMenuManager::MousePointerDemolition(){
    GAP.MenuManager.HideBuildingMenus();
    mouseIsDemolition = true;
}

void CMenuManager::ShowInfoTile(tile_weak_ptr ptr){
    if(InfoMenus.size() && !showingTile){
        return;
    }
    InfoMenus.clear();
    showingTile = true;
    if(auto s = ptr.lock()){
        if(s->GetResource()){
            CInfoMenu infoMenu = CInfoMenu(ptr);
            InfoMenus.push_back(infoMenu);
            return;
        }
    }
}

void CMenuManager::ShowInfoUnit(unit_weak_ptr ptr){
    showingTile = false;
    InfoMenus.clear();
    SetHighLightCirce(build_weak_ptr());
    CInfoMenu infoMenu = CInfoMenu(ptr);
    InfoMenus.push_back(infoMenu);
}

void CMenuManager::ShowInfoBuilding(build_weak_ptr ptr){
    showingTile = false;
    InfoMenus.clear();
    SetHighLightCirce(ptr);
    CInfoMenu infoMenu = CInfoMenu(ptr);
    InfoMenus.push_back(infoMenu);
}

#include "CInfoMenu.h"
#include "CGame.h"


extern CGame GAP;

CInfoMenu::CInfoMenu(unit_weak_ptr ptr){
    myUnit = ptr;
    myBuilding.reset();
    w = CScreen::screen_w / 5;
    x = CScreen::screen_w - w;
    y = 0;
    h = CScreen::screen_h;
    imageBox.x = x + 30;
    imageBox.y = y + 30;
    imageBox.w = w - 60;
    imageBox.h = w - 60;
}

CInfoMenu::CInfoMenu(build_weak_ptr ptr){
    myBuilding = ptr;
    myUnit.reset();
    w = CScreen::screen_w / 5;
    x = CScreen::screen_w - w;
    y = 0;
    h = CScreen::screen_h;
    imageBox.x = x + 10;
    imageBox.y = y + 10;
    imageBox.w = w - 20;
    imageBox.h = w - 20;
}

CInfoMenu::CInfoMenu(tile_weak_ptr ptr){
    myBuilding.reset();
    myUnit.reset();
    myTile = ptr;
    w = CScreen::screen_w / 5;
    x = CScreen::screen_w - w;
    y = 0;
    h = w ;
    imageBox.x = x + 10;
    imageBox.y = y + 10;
    imageBox.w = w - 20;
    imageBox.h = w / 2;
}

CInfoMenu::~CInfoMenu(){
    //dtor
}

void CInfoMenu::Render(){
    PopUpButtons.clear();
    SDL_Color grey = {100, 100, 100, 255};
    SDL_Color red = {255, 100, 100, 255};
    SDL_Color green = {100, 255, 100, 255};
    SDL_Color black = {0, 0, 0, 255};
    GPU_RectangleFilled(GAP.MainGLWindow,x,y,x + w,y + h,grey);
    yOff = imageBox.h + imageBox.y + 10 + yScroll;
    GPU_Rect itemRect;
    itemRect.w = 20;
    itemRect.h = 20;

    std::stringstream tmp;
    if(auto s = myTile.lock()){
        tmp << s->GetResourceAmount() << " " << CGood::GetResourceName(s->GetResource());
        CTree resource = CTree();
        resource.Set(imageBox.x + 50, imageBox.y + 80, s->GetResource(), s->GetVariety());
        resource.Render(true);
        std::string strings = tmp.str();
        RenderLine(tmp.str().c_str(), 16);
    }
    if(auto s = myUnit.lock()){
        RenderLine("Unit Overview:", 16);
        tmp << "TileX: " << s->GetTileX() << " TileY: " << s->GetTileY();
        RenderLine(tmp.str().c_str(), 16);
        tmp.str(std::string());
        RenderLine(*(s->GetThought()), 16);
        if(s->GetCarriedItem()){
            tmp << "Carrying 1 " << CGood::GetResourceName(s->GetCarriedItem());
            RenderLine(tmp.str().c_str(), 16);
            tmp.str(std::string());
        }

        RenderLine("Inventory:", 16);
        int counter = 0;
        itemRect.x = x;
        itemRect.y = yOff;
        for(auto const& item : *(s->GetInventory()) ){
            if(item.second > 0 ){
                counter++;
                CGood::Render(&itemRect, item.first, item.second, true);
                if(counter > 2){
                    itemRect.x = x;
                    yOff += itemRect.h;
                    itemRect.y = yOff;
                    counter = 0;
                }else{
                    itemRect.x += itemRect.w * 4;
                }
            }
        }
        yOff += itemRect.h;

        tmp << "Actions queued: " << s->GetActionCount();
        RenderLine(tmp.str().c_str(), 16);
        tmp.str(std::string());
        RenderLine("Home:", 16);
        if(auto homeb = s->GetHome().lock()){
            CButton button = CButton(x + 10, yOff, 1, s->GetHome() );
            yOff += button.GetH();
            PopUpButtons.push_back(button);
            GAP.TextureManager.DrawConnectionLine(s->GetTileX(),s->GetTileY(),homeb->GetDoor().first,homeb->GetDoor().second, green);
        }
        RenderLine("Workplace:", 16);
        if(auto homeb = s->GetWorkplace().lock()){
            CButton button = CButton(x + 10, yOff, 1, s->GetWorkplace() );
            yOff += button.GetH();
            PopUpButtons.push_back(button);
            GAP.TextureManager.DrawConnectionLine(s->GetTileX(),s->GetTileY(),homeb->GetDoor().first,homeb->GetDoor().second, red);
        }
        CMenu::Render();
        s->RenderInPosition(&imageBox);
    }

    if(auto s = myBuilding.lock()){
        RenderLine("Building Overview:", 16);
        RenderLine(*(s->GetName()), 16);
        RenderLine(*(s->GetDescription()), 16);
        tmp << "DoorX: " << s->GetDoor().first << " DoorY: " << s->GetDoor().second;
        RenderLine(tmp.str().c_str(), 16);
        tmp.str(std::string());
        RenderLine("Inventory:", 16);
        int counter = 0;
        itemRect.x = x;
        itemRect.y = yOff;
        for(auto const& item : *(s->GetInventory()) ){
            if(item.second > 0 || ( s->UnderConstruction() && s->GetMaxStorage(item.first, true) ) ){
                counter++;
                CGood::Render(&itemRect, item.first, item.second, true);
                if(counter > 2){
                    itemRect.x = x;
                    yOff += itemRect.h;
                    itemRect.y = yOff;
                    counter = 0;
                }else{
                    itemRect.x += itemRect.w * 4;
                }
            }
        }
        yOff += itemRect.h;
        if(s->UnderConstruction()){
            tmp << "Work needed: " << s->UnderConstruction();
            RenderLine(tmp.str().c_str(), 16);
            tmp.str(std::string());
        }
        for(auto wb : *(s->GetConnections())){
            if(auto sb = wb.lock()){
                GAP.TextureManager.DrawConnectionLine(sb->GetDoor().first ,sb->GetDoor().second ,s->GetDoor().first,s->GetDoor().second, grey);
            }
        }
        if(s->DistributionRange()){
            RenderLine("Connected Towns:", 16);
            for(auto wb : *(s->GetConnectedTowns())){
                if(auto sb = wb.lock()){
                    CButton button = CButton(x + 10, yOff, 1, wb );
                    yOff += button.GetH();
                    PopUpButtons.push_back(button);
                    GAP.TextureManager.DrawConnectionLine(sb->GetDoor().first,sb->GetDoor().second, s->GetDoor().first,s->GetDoor().second, green);
                }
            }
        }
        RenderLine("Inhabitants:", 16);
        for(auto wu : s->GetInhabitants()){
            if(auto su = wu.lock()){
                CButton button = CButton(x + 10, yOff, 1, wu );
                yOff += button.GetH();
                PopUpButtons.push_back(button);
                GAP.TextureManager.DrawConnectionLine(su->GetTileX(),su->GetTileY(),s->GetDoor().first,s->GetDoor().second, green);
            }
        }
        if(s->MaxWorkers()){
            RenderLine("Workers:", 16);
            GPU_Line(GAP.MainGLWindow, x + 10, yOff + 10, x + 210, yOff + 10, black);
            CButton button = CButton(x + 10 + (s->MaxAssignedWorkers()*(200/s->MaxWorkers())), yOff, 1, 20, 20, myBuilding );
            yOff += button.GetH();
            PopUpButtons.push_back(button);
            for(auto wu : s->GetWorkers()){
                if(auto su = wu.lock()){
                    CButton button = CButton(x + 10, yOff, 1, wu );
                    yOff += button.GetH();
                    PopUpButtons.push_back(button);
                    GAP.TextureManager.DrawConnectionLine(su->GetTileX(),su->GetTileY(),s->GetDoor().first,s->GetDoor().second, red);
                }
            }
        }
        RenderLine("Production:", 16);
        for(auto & recipe : *(s->GetRecipes())){
            if(!recipe.IsAvailable()){
                continue;
            }
            tmp << *(recipe.GetName());
            RenderLine(tmp.str().c_str(), 10);
            tmp.str(std::string());
            GPU_Line(GAP.MainGLWindow, x + 10, yOff + 10, x + 210, yOff + 10, black);
            CButton button = CButton(x + 10 + (recipe.GetProductionPrio()*20), yOff, 1, 20, 20, recipe );
            yOff += button.GetH();
            PopUpButtons.push_back(button);
        }
        CMenu::Render();
        s->RenderInPosition(&imageBox);
    }


}

void CInfoMenu::RenderLine(std::string text, int fontSize){

    SDL_Color black = {0, 0, 0};
    SDL_Surface* textSurface = TTF_RenderText_Solid( GAP.TextureManager.GetFont(30), text.c_str(), black );
    GPU_Image* texture = GPU_CopyImageFromSurface( textSurface );
    GPU_Rect Message_rect; //create a rect
    Message_rect.x = x + 10;  //controls the rect's x coordinate
    Message_rect.y = y + yOff; // controls the rect's y coordinte
    Message_rect.w = strlen(text.c_str())*10; // controls the width of the rect
    Message_rect.h = 20; // controls the height of the rect

    GAP.TextureManager.DrawTextureGL(texture, 0, &Message_rect, true);
    GPU_FreeImage(texture);
    SDL_FreeSurface( textSurface );
    yOff += 20;
}

void CInfoMenu::Clicked(CButton button){

    GAP.MenuManager.HideContextMenus();

    unit_weak_ptr unit_ptr = button.GetUnit();
    if(auto s = unit_ptr.lock()){
        GAP.MenuManager.ShowInfoUnit(unit_ptr);
        return;
    }

    build_weak_ptr build_ptr = button.GetBuilding();
    if(auto s = build_ptr.lock()){
        GAP.MenuManager.ShowInfoBuilding(build_ptr);
        return;
    }

    build_weak_ptr workers_build_ptr = button.GetWorkerBuilding();
    if(auto s = workers_build_ptr.lock()){
        selectecWorkerBuilding = workers_build_ptr;
        return;
    }

    CRecipe* recipe = button.GetRecipe();
    if(recipe != nullptr){
        selectedRecipe = recipe;
        return;
    }
}

bool CInfoMenu::HandleLClickUp(int x_, int y_){
    selectedRecipe = nullptr;
    selectecWorkerBuilding = build_weak_ptr();
    return true;
}

bool CInfoMenu::HandleMouseMovement(int x_, int y_){
    if(auto s = selectecWorkerBuilding.lock()){
        s->MaxAssignedWorkers((x_ - (x + 10)) / (200/s->MaxWorkers()));
    }
    if(selectedRecipe != nullptr){
        int currentPrio = selectedRecipe->GetProductionPrio();
        int newVal = (x_ - (x + 10)) / 20;
        if(newVal < 0){
            newVal = 0;
        }
        if(newVal > 10){
            newVal = 10;
        }
        if(newVal != currentPrio){
            selectedRecipe->SetProductionPrio(newVal);
        }
        return true;
    }
    return false;
}

bool CInfoMenu::OnMouseWheel(bool Up, int x_, int y_){
    if(x_ < x){
        return false;
    }
    if(!Up){
        yScroll -= 30;
    }else{
        yScroll += 30;
        if(yScroll > 0){
            yScroll = 0;
        }
    }
    return true;
}






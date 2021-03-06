#include "CTile.h"
#include "CGame.h"

extern CGame GAP;

std::string CTile::spriteSheet = "tiles";

void CTile::Init(int type, int tx, int ty, int resource_, int resourceAmount_, int resourceVariety_){
    x = tx * tileWidth;
    y = ty * tileWidth;
    tileX = tx;
    tileY = ty;
    box.h = tileWidth;
    box.w = tileWidth;
    box.x = x;
    box.y = y;
    SetTerrain(type);
    resource = resource_;
    resourceAmount = resourceAmount_;
    resourceVariety = resourceVariety_;
    if(resource_){
        resourcePtr = tree_uniq_ptr(new CTree());
        resourcePtr->Set(x, y, resource_, resourceVariety_);
    }
}

void CTile::SetTerrain(int type){
    terrain = type;
    clip.w = tileWidth;
    clip.h = tileWidth;
    switch(type){
        case 92: clip.x = 256; clip.y = 0; break;
        case 93: clip.x = 256; clip.y = 32; break;
        case 94: clip.x = 256; clip.y = 64; break;
        case 95: clip.x = 256; clip.y = 96; break;
        case 96: clip.x = 256; clip.y = 128; break;



    }
}

bool CTile::RenderOnMinimap(int x, int y){
    return true;
}

SDL_Color CTile::GetMinimapColor(){
    SDL_Color color;
    switch(terrain){
        case 92: color.r = 255; color.b = 255; color.g = 255; break;
        case 93: color.r = 255; color.b = 255; color.g = 255; break;
        case 94: color.r = 255; color.b = 255; color.g = 255; break;
        case 95: color.r = 255; color.b = 255; color.g = 255; break;
        case 96: color.r = 255; color.b = 255; color.g = 255; break;
    }
    return color;
}

bool CTile::Render(int row, int col){
    // GAP.TextureManager.DrawTextureGL(&spriteSheet, &clip, &box);
    RenderResource();
    RenderUnits();
    if(row){
        if(rightNeighbour){
            rightNeighbour->Render(row - 1);
        }
    }
    if(col){
        if(downNeighbour){
            downNeighbour->Render(row, col - 1);
        }
    }
    return 1;
}

bool CTile::RenderResource(){
    if(!resource){
        return false;
    }
    resourcePtr->Render();
    return true;
}

bool CTile::RenderUnits(){
    for(auto e : Units){
        if(auto s = e.lock()){
            s->Render();
        }
    }
    return 1;
}

void CTile::AddUnit(unit_weak_ptr ptr){
    if(owner == 0){
        if(auto s = ptr.lock()){
            owner = s->Owner();
        }
    }
    Units.push_back(ptr);
}

void CTile::RemoveUnit(int id){
    std::vector<unit_weak_ptr>::iterator iter = Units.begin();
    while (iter != Units.end())
    {
        if(auto s = (*iter).lock()){
            if(s->GetId() == id){
                Units.erase(iter);
                if(Units.size() < 1){
                    owner = 0;
                }
                return;
            }
        }
        iter++;
    }
    CLog::Write("Trying to remove nonexistant unit from tile?");
}

int CTile::HarvestResource(int amount){
    int harvestedAmount = 0;
    if(!resource){
        return harvestedAmount;
    }
    if(resourceAmount == 0){
        return harvestedAmount;
    }
    if(resourceAmount < amount){
        harvestedAmount = resourceAmount;
    }else{
        harvestedAmount = amount;
    }
    resourceAmount -= harvestedAmount;
    if(resourceAmount <= 0){
        resource = 0;
        GAP.Pathfinder.SetCost(GetTileX(), GetTileY(), CScreen::flatMoveCost );
    }
    return harvestedAmount;
}

std::vector<unit_weak_ptr> CTile::UnitsAtTile(){
    return Units;
}

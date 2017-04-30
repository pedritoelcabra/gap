#include "CTile.h"
#include "CGame.h"

extern CGame GAP;

CTile::CTile(){
}

CTile::~CTile(){
    //dtor
}

void CTile::Init(int type, int tx, int ty, int resource_, int resourceAmount_){
    x = tx * tileWidth;
    y = ty * tileWidth;
    tileX = tx;
    tileY = ty;
    box.h = tileWidth * 2;
    box.w = tileWidth * 2;
    box.x = x;
    box.y = y;
    setTerrain(type);
    resource = resource_;
    resourceAmount = resourceAmount_;
}

void CTile::setTerrain(int type){
    terrain = type;
    clip.w = 128;
    clip.h = 128;
    switch(type){
        case 1: clip.x = 0; clip.y = 0; break;
        case 2: clip.x = 0; clip.y = 128; break;
        case 3: clip.x = 128; clip.y = 0; break;
        case 4: clip.x = 128; clip.y = 128; break;

        case 92: clip.x = 0; clip.y = 256; break;
        case 93: clip.x = 128; clip.y = 256; break;
        case 94: clip.x = 0; clip.y = 384; break;
        case 95: clip.x = 128; clip.y = 384; break;
        case 96: clip.x = 0; clip.y = 512; break;
        case 97: clip.x = 128; clip.y = 512; break;
        case 98: clip.x = 0; clip.y = 640; break;
        case 99: clip.x = 128; clip.y = 640; break;



    }
}

bool CTile::RenderOnMinimap(int x, int y){
    SDL_SetRenderDrawColor(GAP.getRenderer(),255, 100, 100, 255);
    SDL_RenderDrawPoint(GAP.getRenderer(), CScreen::screen_w - 120 + x , 20 + y);
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

void CTile::setMoveCost(float cost_){
    moveCost = cost_;
}

float CTile::getMoveCost(){
    return moveCost;
}

int CTile::getResource(){
    return resource;
}

bool CTile::Render(){
    GAP.TextureManager.DrawTextureGL("tiles", &clip, &box);
    return 1;
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
    Units.push_back(ptr);
}

void CTile::RemoveUnit(int id){
    std::vector<unit_weak_ptr>::iterator iter = Units.begin();
    while (iter != Units.end())
    {
        if(auto s = (*iter).lock()){
            if(s->GetId() == id){
                Units.erase(iter);
                return;
            }
        }
        iter++;
    }
    CLog::Write("Trying to remove nonexistant unit from tile?");
}

int CTile::HarvestResource(){
    if(!resource){
        return 0;
    }
    if(!resourceAmount){
        return 0;
    }
    resourceAmount--;
    if(!resourceAmount){
        resource = 0;
        GAP.Pathfinder.SetCost(getTileX(), getTileY(), CScreen::flatMoveCost );
    }
    return 1;
}

std::vector<unit_weak_ptr> CTile::UnitsAtTile(){
    return Units;
}

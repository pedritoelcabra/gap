

#include "CChunk.h"
#include "CGame.h"


extern CGame GAP;

CChunk::CChunk()
{

}

CChunk::~CChunk()
{
    //dtor
}


void CChunk::Init(int chunkX, int chunkY, utils::NoiseMap* heightMap) //
{
    cx = chunkX;
    cy = chunkY;
    Tiles.clear();
    float height = 0;
    int terrain = 0;
    int x, y, resource, resourceRand, resourceAmount;
    float moveCost = 0.0f;
    for(int k = 0; k <= tilesPerChunk; k++){
        for(int i = 0; i <= tilesPerChunk; i++){
            x = (chunkX * tilesPerChunk) + i;
            y = (chunkY * tilesPerChunk) + k;
            height = heightMap->GetValue(k, i);
            if(height < -0.7){
                terrain = 92;
                moveCost = 4.0f;
            }else if(height < -0.5){
                terrain = 93;
                moveCost = CScreen::flatMoveCost;
            }else if(height < 0.2){
                terrain = 94;
                moveCost = CScreen::flatMoveCost;
            }else if(height < 0.7){
                terrain = 95;
                moveCost = CScreen::flatMoveCost;
            }else{
                terrain = 96;
                moveCost = 5.0f;
            }
            resource = 0;
            if(terrain > 93 && terrain < 96){
                resourceRand = rand() % 1000 ;
                if( resourceRand < 25 ){
                    resource = 1;
                    resourceAmount = 1;
                    moveCost = 3.0f;
                }
            }
            Tiles[y][x] = std::make_shared<CTile>(CTile());
            Tiles[y][x]->Init(terrain, x, y, resource, resourceAmount);
            Tiles[y][x]->setMoveCost(moveCost);
            GAP.Pathfinder.SetCost(x,y,moveCost);
        }
    }
}

tile_weak_ptr CChunk::GetTile(int x, int y){
    return tile_weak_ptr(Tiles.at(y).at(x));
}

void CChunk::AddUnit( unit_weak_ptr ptr){
    Units.push_back(ptr);
}

void CChunk::RemoveUnit(int id){
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
}

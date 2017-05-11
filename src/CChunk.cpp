

#include "CChunk.h"
#include "CGame.h"


extern CGame GAP;

void CChunk::Init(int chunkX, int chunkY, utils::NoiseMap* heightMap, utils::NoiseMap* heightMapForest, utils::NoiseMap* heightMapStones){
    cx = chunkX;
    cy = chunkY;
    offX = (chunkX * tilesPerChunk);
    offY = (chunkY * tilesPerChunk);
    float height = 0;
    int terrain = 0;
    int x, y, resource, resourceRand, resourceAmount;
    float moveCost = 0.0f;
    for(int k = 0; k < tilesPerChunk; k++){
        for(int i = 0; i < tilesPerChunk; i++){
            x = (chunkX * tilesPerChunk) + i;
            y = (chunkY * tilesPerChunk) + k;
            height = heightMap->GetValue(k, i);
            if(height < CScreen::waterLevel){
                terrain = 92;
                moveCost = 4.0f;
            }else if(height < CScreen::beachLevel){
                terrain = 93;
                moveCost = CScreen::flatMoveCost;
            }else if(height < CScreen::grassLevel){
                terrain = 94;
                moveCost = CScreen::flatMoveCost;
            }else if(height < CScreen::dryLandLevel){
                terrain = 95;
                moveCost = CScreen::flatMoveCost;
            }else{
                terrain = 96;
                moveCost = 5.0f;
            }
            resource = 0;
            if(terrain > 93 && terrain < 96){
                resourceRand = rand() % 2000 ;
                if( (heightMapForest->GetValue(k, i) * 1000) + resourceRand > 2100 ){
                    resource = 1;
                    resourceAmount = 1;
                    moveCost = 3.0f;
                }
                if( (heightMapStones->GetValue(k, i) * 10000) + resourceRand > 11500 ){
                    resource = 2;
                    resourceAmount = 1;
                    moveCost = 3.0f;
                }
            }
            Tiles[k][i] = std::make_shared<CTile>();
            Tiles[k][i]->Init(terrain, x, y, resource, resourceAmount, rand() % 4);
            Tiles[k][i]->SetMoveCost(moveCost);
            GAP.Pathfinder.SetCost(x,y,moveCost);
        }
    }
    // right
    for(int k = 0; k < tilesPerChunk; k++){
        for(int i = 0; i < tilesPerChunk - 1; i++){
            Tiles[k][i]->SetRightNeighbour(tile_weak_ptr(Tiles[k][i+1]));
        }
    }
    for(int k = 0; k < tilesPerChunk - 1; k++){
        Tiles[k][tilesPerChunk-1]->SetRightNeighbour(GAP.ChunkManager.GetTile(((chunkX + 1) * tilesPerChunk), (chunkY * tilesPerChunk) + k));
    }
    for(int k = 0; k < tilesPerChunk; k++){
        if(auto t = GAP.ChunkManager.GetTile((chunkX * tilesPerChunk) - 1, (chunkY * tilesPerChunk) + k).lock()){
            t->SetRightNeighbour(tile_weak_ptr(Tiles[k][0]));
        }
    }
    // down

    for(int k = 0; k < tilesPerChunk - 1; k++){
        for(int i = 0; i < tilesPerChunk; i++){
            Tiles[k][i]->SetDownNeighbour(tile_weak_ptr(Tiles[k + 1][i]));
        }
    }
    for(int k = 0; k < tilesPerChunk - 1; k++){
        Tiles[tilesPerChunk-1][k]->SetDownNeighbour(GAP.ChunkManager.GetTile((chunkX * tilesPerChunk) + k, ((chunkY+1) * tilesPerChunk)));
    }
    for(int k = 0; k < tilesPerChunk; k++){
        if(auto t = GAP.ChunkManager.GetTile((chunkX * tilesPerChunk) +k , (chunkY * tilesPerChunk) - 1).lock()){
            t->SetDownNeighbour(tile_weak_ptr(Tiles[0][k]));
        }
    }
    isInited = true;
}

tile_weak_ptr CChunk::GetTile(int x, int y){
    if(!isInited){
        return tile_weak_ptr();
    }
    return tile_weak_ptr(Tiles[y - offY][x - offX]);
}

void CChunk::AddUnit( unit_weak_ptr ptr){
    if(!isInited){
        return;
    }
    Units.push_back(ptr);
}

void CChunk::RemoveUnit(int id){
    if(!isInited){
        return;
    }
    std::vector<unit_weak_ptr>::iterator iter = Units.begin();
    while (iter != Units.end())    {
        if(auto s = (*iter).lock()){
            if(s->GetId() == id){
                Units.erase(iter);
                return;
            }
        }
        iter++;
    }
}

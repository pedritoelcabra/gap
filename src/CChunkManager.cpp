
#include <vector>
#include "CChunkManager.h"
#include "CGame.h"
#include <noise/noise.h>
#include "noiseutils.h"


extern CGame GAP;

void CChunkManager::Init(){
    CLog::Write("Seed:");
    CLog::Write(GAP.GetSeed());

    currentChunkX = 999999;
    currentChunkY = 999999;
    chunkPortion = 10000 * CScreen::tilesPerChunk;
    chunkPortion = chunkPortion / 1000000;

    GenericChunk = std::make_shared<CChunk>();
    for(int k = -40; k < 40 ; k++){
        for(int i = -40; i < 40 ; i++){
            Matrix[k][i] = GenericChunk;
        }
    }

    heightMapBuilder.SetDestSize (CScreen::tilesPerChunk, CScreen::tilesPerChunk);

    myModule.SetFrequency (1.0);
    myModule.SetSeed(GAP.GetSeed());

    myModuleForest.SetFrequency (5.0);
    myModuleForest.SetSeed(GAP.GetSeed());

    myModuleStones.SetFrequency (3.0);
    myModuleStones.SetSeed(GAP.GetSeed());

    bool findingStartingPlace = true;
    xOff = -1;
    while(findingStartingPlace){
        xOff++;
        float chunkY = 0;
        float chunkX = chunkPortion * xOff;
        heightMapBuilder.SetSourceModule (myModule);
        heightMapBuilder.SetBounds (chunkY, chunkY + chunkPortion,chunkX, chunkX + chunkPortion);
        heightMapBuilder.SetDestNoiseMap (heightMap);
        heightMapBuilder.Build ();

        findingStartingPlace = false;
        for(int k = 0; k < tilesPerChunk ; k++){
            for(int i = 0; i < tilesPerChunk ; i++){
                if(heightMap.GetValue(k, i) < CScreen::waterLevel || heightMap.GetValue(k, i) >= CScreen::dryLandLevel){
                    findingStartingPlace = true;
                }
            }
        }
    }

    UpdateChunks(true);

}

void CChunkManager::UpdateChunks(bool forceUpdate){
    if(!forceUpdate && GAP.GetTick() % 5){
        return;
    }
    currentChunkX = GetChunk(GAP.Player->GetTileX());
    currentChunkY = GetChunk(GAP.Player->GetTileY());

    bool hasGenerated;
    for(int i = currentChunkY - 2; i <= currentChunkY + 2; i++ ){
        for(int k = currentChunkX - 2; k <= currentChunkX + 2; k++ ){
            hasGenerated = GenerateChunk(k, i);
            if(hasGenerated && !forceUpdate){
                return;
            }
        }
    }

    return;
}

void CChunkManager::RenderTiles(){

    showWidth = (int)((CScreen::screen_w / CScreen::tileWidth) * GAP.ZoomLvl());
    leftX = GAP.Player->GetTileX() - (showWidth/2);
    rightX = GAP.Player->GetTileX() + (showWidth/2);

    showHeight = (int)((CScreen::screen_h / CScreen::tileWidth) * GAP.ZoomLvl());
    topY = GAP.Player->GetTileY() - (showHeight/2);
    botY = GAP.Player->GetTileY() + (showHeight/2);

    for(int i = GetChunk(topY); i <= GetChunk(botY + 3); i++){
        for(int k = GetChunk(leftX); k <= GetChunk(rightX + 5); k++){
            Matrix[k][i]->RenderChunkTiles();
        }
    }


    return;
}

void CChunkManager::RenderObjects(){

    for(int i = GetChunk(topY); i <= GetChunk(botY + 3); i++){
        for(int k = GetChunk(leftX); k <= GetChunk(rightX + 5); k++){
            Matrix[k][i]->RenderChunkObjects();
        }
    }
}

void CChunkManager::RenderMinimap(){
//    int viewDist = 2 / GAP.ZoomLvl();
//    int mapViewChunkX = GetChunk(GAP.PixelToTile(GAP.MainViewport.x));
//    int mapViewChunkY = GetChunk(GAP.PixelToTile(GAP.MainViewport.y));
//    for(int i = mapViewChunkY - viewDist; i <= mapViewChunkY + viewDist; i++ ){
//        for(int k = mapViewChunkX - viewDist; k <= mapViewChunkX + viewDist; k++ ){
//            Matrix[GetChunk(k)][GetChunk(i)]->RenderChunk();
//        }
//    }
}

bool CChunkManager::ChunkExists(int x, int y){
    if(Matrix[x][y]->IsInited()){
        return true;
    }
    return false;
}

bool CChunkManager::GenerateChunk(int x, int y){
    if(Matrix[x][y]->IsInited()){
        return false;
    }
    float chunkY = chunkPortion * y;
    float chunkX = chunkPortion * (x + xOff);
    CLog::Write("Generating chunk:");
    CLog::Write(chunkX);
    CLog::Write(chunkY);

    heightMapBuilder.SetSourceModule (myModule);
    heightMapBuilder.SetBounds (chunkY, chunkY + chunkPortion,chunkX, chunkX + chunkPortion);
    heightMapBuilder.SetDestNoiseMap (heightMap);
    heightMapBuilder.Build ();

    heightMapBuilder.SetSourceModule (myModuleForest);
    heightMapBuilder.SetBounds (chunkY, chunkY + chunkPortion,chunkX + 220, chunkX + chunkPortion + 220);
    heightMapBuilder.SetDestNoiseMap (heightMapForest);
    heightMapBuilder.Build ();

    heightMapBuilder.SetSourceModule (myModuleStones);
    heightMapBuilder.SetBounds (chunkY + 220, chunkY + chunkPortion + 220,chunkX, chunkX + chunkPortion);
    heightMapBuilder.SetDestNoiseMap (heightMapStones);
    heightMapBuilder.Build ();

    chunk_shared_ptr newChunk = std::make_shared<CChunk>();
    newChunk->Init(x, y, &heightMap, &heightMapForest, &heightMapStones);
    Matrix[x][y] = newChunk;
    return true;
}

void CChunkManager::TilesInTileArea(std::vector<tile_weak_ptr>* tileVec, int tileX, int tileY, int tileW, int tileH){
    tileVec->clear();
    for(int k = tileY; k < (tileY + tileH); k++){
        for(int i = tileX; i < (tileX + tileW); i++){
            tileVec->push_back(GetTile(i,k));
        }
    }
}

std::vector<tile_weak_ptr> CChunkManager::GetResources(int resourceType, int resourceRadius, int centerX, int centerY){
    std::vector<tile_weak_ptr> tiles;
    TilesInTileArea(&tiles, centerX - 1, centerY - 1, 3, 3);
    std::vector<tile_weak_ptr> resources;
    for(auto e : tiles){
        if(auto s = e.lock()){
            if(s->GetResource() == resourceType){
                resources.push_back(e);
            }
        }
    }
    if(resources.size() < 1){

        TilesInTileArea(&tiles, centerX - resourceRadius, centerY - resourceRadius, (resourceRadius * 2) + 1, (resourceRadius * 2) + 1);
        for(auto e : tiles){
            if(auto s = e.lock()){
                if(s->GetResource() == resourceType){
                    if(s->GetTileFlightRoundDistance(centerX, centerY) <= resourceRadius){
                        resources.push_back(e);
                    }
                }
            }
        }
    }
    return resources;
}

tile_weak_ptr CChunkManager::GetTile(int x, int y){
    return Matrix[GetChunk(x)][GetChunk(y)]->GetTile(x,y);
}

int CChunkManager::GetChunk(int tile){
    if(tile < 0){
        return ((tile+1)/tilesPerChunk) - 1;
    }
    return tile/tilesPerChunk;
}

void CChunkManager::MoveUnit(int x, int y, int nx, int ny, unit_weak_ptr ptr, int id, bool removeUnit){
    int ocx = GetChunk(x);
    int ocy = GetChunk(y);
    int ncx = GetChunk(nx);
    int ncy = GetChunk(ny);
    tile_weak_ptr optr = GetTile(x,y);
    tile_weak_ptr nptr = GetTile(nx,ny);
    if(auto o = optr.lock()){
        if(auto n = nptr.lock()){
            o->RemoveUnit(id);
            if(!removeUnit){
                n->AddUnit(ptr);
            }
        }
    }
    if(ocx == ncx && ocy == ncy){
        return;
    }
    Matrix[GetChunk(ocx)][GetChunk(ocy)]->RemoveUnit(id);
    if(!removeUnit){
        Matrix[GetChunk(ncx)][GetChunk(ncy)]->AddUnit(ptr);
    }
}

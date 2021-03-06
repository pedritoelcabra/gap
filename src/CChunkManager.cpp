
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

    myModule.SetFrequency (GAP.SettingF(CSettingManager::TerrainFrequency));
    myModule.SetSeed(GAP.GetSeed());

    myModuleForest.SetFrequency (GAP.SettingF(CSettingManager::ForestFrequency));
    myModuleForest.SetSeed(GAP.GetSeed());

    myModuleStones.SetFrequency (GAP.SettingF(CSettingManager::StoneFrequency));
    myModuleStones.SetSeed(GAP.GetSeed());

    myModuleCopper.SetFrequency (GAP.SettingF(CSettingManager::CopperFrequency));
    myModuleCopper.SetSeed(GAP.GetSeed());

    myModuleTin.SetFrequency (GAP.SettingF(CSettingManager::TinFrequency));
    myModuleTin.SetSeed(GAP.GetSeed());

    myModuleCoal.SetFrequency (GAP.SettingF(CSettingManager::CoalFrequency));
    myModuleCoal.SetSeed(GAP.GetSeed());

    myModuleGold.SetFrequency (GAP.SettingF(CSettingManager::GoldFrequency));
    myModuleGold.SetSeed(GAP.GetSeed());

    myModuleSilver.SetFrequency (GAP.SettingF(CSettingManager::SilverFrequency));
    myModuleSilver.SetSeed(GAP.GetSeed());

    myModuleIron.SetFrequency (GAP.SettingF(CSettingManager::IronFrequency));
    myModuleIron.SetSeed(GAP.GetSeed());

    myModuleLapis.SetFrequency (GAP.SettingF(CSettingManager::LapisFrequency));
    myModuleLapis.SetSeed(GAP.GetSeed());

    myModuleGems.SetFrequency (GAP.SettingF(CSettingManager::GemFrequency));
    myModuleGems.SetSeed(GAP.GetSeed());

    myModuleEbony.SetFrequency (GAP.SettingF(CSettingManager::EbonyFrequency));
    myModuleEbony.SetSeed(GAP.GetSeed());

    myModuleLime.SetFrequency (GAP.SettingF(CSettingManager::LimeFrequency));
    myModuleLime.SetSeed(GAP.GetSeed());

    myModuleMarble.SetFrequency (GAP.SettingF(CSettingManager::MarbleFrequency));
    myModuleMarble.SetSeed(GAP.GetSeed());

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
                if(heightMap.GetValue(k, i) < GAP.SettingF(CSettingManager::WaterLevel)
                   || heightMap.GetValue(k, i) >= GAP.SettingF(CSettingManager::DryLandLevel)){
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
    for(int i = currentChunkY - GAP.Setting(CSettingManager::ChunkGenerationDistance)
        ; i <= currentChunkY + GAP.Setting(CSettingManager::ChunkGenerationDistance); i++ ){
        for(int k = currentChunkX - GAP.Setting(CSettingManager::ChunkGenerationDistance);
        k <= currentChunkX + GAP.Setting(CSettingManager::ChunkGenerationDistance); k++ ){
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
    chunkX += 110;
    chunkY += 220;

    heightMapBuilder.SetSourceModule (myModuleForest);
    heightMapBuilder.SetBounds (chunkY, chunkY + chunkPortion,chunkX, chunkX + chunkPortion);
    heightMapBuilder.SetDestNoiseMap (heightMapForest);
    heightMapBuilder.Build ();
    chunkX += 110;
    chunkY += 220;

    heightMapBuilder.SetSourceModule (myModuleStones);
    heightMapBuilder.SetBounds (chunkY, chunkY + chunkPortion,chunkX, chunkX + chunkPortion);
    heightMapBuilder.SetDestNoiseMap (heightMapStones);
    heightMapBuilder.Build ();
    chunkX += 110;
    chunkY += 220;

    heightMapBuilder.SetSourceModule (myModuleCopper);
    heightMapBuilder.SetBounds (chunkY, chunkY + chunkPortion,chunkX, chunkX + chunkPortion);
    heightMapBuilder.SetDestNoiseMap (heightMapCopper);
    heightMapBuilder.Build ();
    chunkX += 110;
    chunkY += 220;

    heightMapBuilder.SetSourceModule (myModuleCoal);
    heightMapBuilder.SetBounds (chunkY, chunkY + chunkPortion,chunkX, chunkX + chunkPortion);
    heightMapBuilder.SetDestNoiseMap (heightMapCoal);
    heightMapBuilder.Build ();
    chunkX += 110;
    chunkY += 220;

    heightMapBuilder.SetSourceModule (myModuleTin);
    heightMapBuilder.SetBounds (chunkY, chunkY + chunkPortion,chunkX, chunkX + chunkPortion);
    heightMapBuilder.SetDestNoiseMap (heightMapTin);
    heightMapBuilder.Build ();
    chunkX += 110;
    chunkY += 220;

    heightMapBuilder.SetSourceModule (myModuleSilver);
    heightMapBuilder.SetBounds (chunkY, chunkY + chunkPortion,chunkX, chunkX + chunkPortion);
    heightMapBuilder.SetDestNoiseMap (heightMapSilver);
    heightMapBuilder.Build ();
    chunkX += 110;
    chunkY += 220;

    heightMapBuilder.SetSourceModule (myModuleGold);
    heightMapBuilder.SetBounds (chunkY, chunkY + chunkPortion,chunkX, chunkX + chunkPortion);
    heightMapBuilder.SetDestNoiseMap (heightMapGold);
    heightMapBuilder.Build ();
    chunkX += 110;
    chunkY += 220;

    heightMapBuilder.SetSourceModule (myModuleIron);
    heightMapBuilder.SetBounds (chunkY, chunkY + chunkPortion,chunkX, chunkX + chunkPortion);
    heightMapBuilder.SetDestNoiseMap (heightMapIron);
    heightMapBuilder.Build ();
    chunkX += 110;
    chunkY += 220;

    heightMapBuilder.SetSourceModule (myModuleLapis);
    heightMapBuilder.SetBounds (chunkY, chunkY + chunkPortion,chunkX, chunkX + chunkPortion);
    heightMapBuilder.SetDestNoiseMap (heightMapLapis);
    heightMapBuilder.Build ();
    chunkX += 110;
    chunkY += 220;

    heightMapBuilder.SetSourceModule (myModuleGems);
    heightMapBuilder.SetBounds (chunkY, chunkY + chunkPortion,chunkX, chunkX + chunkPortion);
    heightMapBuilder.SetDestNoiseMap (heightMapGems);
    heightMapBuilder.Build ();
    chunkX += 110;
    chunkY += 220;

    heightMapBuilder.SetSourceModule (myModuleEbony);
    heightMapBuilder.SetBounds (chunkY, chunkY + chunkPortion,chunkX, chunkX + chunkPortion);
    heightMapBuilder.SetDestNoiseMap (heightMapEbony);
    heightMapBuilder.Build ();
    chunkX += 110;
    chunkY += 220;

    heightMapBuilder.SetSourceModule (myModuleLime);
    heightMapBuilder.SetBounds (chunkY, chunkY + chunkPortion,chunkX, chunkX + chunkPortion);
    heightMapBuilder.SetDestNoiseMap (heightMapLime);
    heightMapBuilder.Build ();
    chunkX += 110;
    chunkY += 220;

    heightMapBuilder.SetSourceModule (myModuleMarble);
    heightMapBuilder.SetBounds (chunkY, chunkY + chunkPortion,chunkX, chunkX + chunkPortion);
    heightMapBuilder.SetDestNoiseMap (heightMapMarble);
    heightMapBuilder.Build ();

    Matrix[x][y] = std::make_shared<CChunk>();
    Matrix[x][y]->Init(x, y);
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

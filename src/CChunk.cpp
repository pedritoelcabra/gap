

#include "CChunk.h"
#include "CGame.h"


extern CGame GAP;

void CChunk::Init(int chunkX, int chunkY){

    SDL_Surface* targetSurf;
    SDL_Surface* sourceSurf;
    targetSurf = SDL_CreateRGBSurfaceWithFormat(0, tilesPerChunk * CScreen::tileWidth,
                                          tilesPerChunk * CScreen::tileWidth,
                                          32, SDL_PIXELFORMAT_RGBA32);
    std::string tileSheetImage = CTile::spriteSheet;
    sourceSurf = GPU_CopySurfaceFromImage(GAP.TextureManager.GetTexture(&tileSheetImage));

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
            height = GAP.ChunkManager.HeightMap()->GetValue(k, i);
            if(height < GAP.SettingF(CSettingManager::WaterLevel)){
                terrain = 92;
                moveCost = 4.0f;
            }else if(height < GAP.SettingF(CSettingManager::BeachLevel)){
                terrain = 93;
                moveCost = CScreen::flatMoveCost;
            }else if(height < GAP.SettingF(CSettingManager::GrassLevel)){
                terrain = 94;
                moveCost = CScreen::flatMoveCost;
            }else if(height < GAP.SettingF(CSettingManager::DryLandLevel)){
                terrain = 95;
                moveCost = CScreen::flatMoveCost;
            }else{
                terrain = 96;
                moveCost = 5.0f;
            }
            resource = 0;
            resourceAmount = 0;
            if(terrain == 95 && height >= GAP.SettingF(CSettingManager::MineralLevel)){

                if( resource == 0 &&
                   (GAP.ChunkManager.CopperMap()->GetValue(k, i) * GAP.Setting(CSettingManager::CopperRand)) + resourceRand
                   > GAP.Setting(CSettingManager::CopperRoof) ){
                    resource = CGood::copperOre;
                    resourceAmount = GAP.Setting(CSettingManager::CopperPerBlock);
                }
                if( resource == 0 &&
                   (GAP.ChunkManager.TinMap()->GetValue(k, i) * GAP.Setting(CSettingManager::TinRand)) + resourceRand
                   > GAP.Setting(CSettingManager::TinRoof) ){
                    resource = CGood::tinOre;
                    resourceAmount = GAP.Setting(CSettingManager::TinPerBlock);
                }
                if( resource == 0 &&
                   (GAP.ChunkManager.CoalMap()->GetValue(k, i) * GAP.Setting(CSettingManager::CoalRand)) + resourceRand
                   > GAP.Setting(CSettingManager::CoalRoof) ){
                    resource = CGood::coal;
                    resourceAmount = GAP.Setting(CSettingManager::CoalPerBlock);
                }
                if( resource == 0 &&
                   (GAP.ChunkManager.SilverMap()->GetValue(k, i) * GAP.Setting(CSettingManager::SilverRand)) + resourceRand
                   > GAP.Setting(CSettingManager::SilverRoof) ){
                    resource = CGood::silverOre;
                    resourceAmount = GAP.Setting(CSettingManager::SilverPerBlock);
                }
                if( resource == 0 &&
                   (GAP.ChunkManager.GoldMap()->GetValue(k, i) * GAP.Setting(CSettingManager::GoldRand)) + resourceRand
                   > GAP.Setting(CSettingManager::GoldRoof) ){
                    resource = CGood::goldOre;
                    resourceAmount = GAP.Setting(CSettingManager::GoldPerBlock);
                }
                if( resource == 0 &&
                   (GAP.ChunkManager.IronMap()->GetValue(k, i) * GAP.Setting(CSettingManager::IronRand)) + resourceRand
                   > GAP.Setting(CSettingManager::IronRoof) ){
                    resource = CGood::ironOre;
                    resourceAmount = GAP.Setting(CSettingManager::IronPerBlock);
                }
            }
            if(terrain == 94 ){
                if( resource == 0 &&
                   (GAP.ChunkManager.EbonyMap()->GetValue(k, i) * GAP.Setting(CSettingManager::EbonyRand)) + resourceRand
                   > GAP.Setting(CSettingManager::EbonyRoof) ){
                    resource = CGood::ebony;
                    resourceAmount = GAP.Setting(CSettingManager::EbonyPerBlock);
                }
            }
            if(terrain == 94 || terrain == 95){
                int maxRand = GAP.Setting(CSettingManager::ResourceRand);
                resourceRand = rand() % maxRand ;
                if( resource == 0 &&
                   (GAP.ChunkManager.ForestMap()->GetValue(k, i) * GAP.Setting(CSettingManager::ForestRand)) + resourceRand
                   > GAP.Setting(CSettingManager::ForestRoof) ){
                    resource = CGood::wood;
                    resourceAmount = GAP.Setting(CSettingManager::WoodPerTree);
                }
                if( resource == 0 &&
                   (GAP.ChunkManager.StoneMap()->GetValue(k, i) * GAP.Setting(CSettingManager::StoneRand)) + resourceRand
                   > GAP.Setting(CSettingManager::StoneRoof) ){
                    resource = CGood::stone;
                    resourceAmount = GAP.Setting(CSettingManager::StonePerBlock);
                }
                if( resource == 0 &&
                   (GAP.ChunkManager.GemsMap()->GetValue(k, i) * GAP.Setting(CSettingManager::GemRand)) + resourceRand
                   > GAP.Setting(CSettingManager::GemRoof) ){
                    resource = CGood::gemstones;
                    resourceAmount = GAP.Setting(CSettingManager::GemPerBlock);
                }
                if( resource == 0 &&
                   (GAP.ChunkManager.LapisMap()->GetValue(k, i) * GAP.Setting(CSettingManager::LapisRand)) + resourceRand
                   > GAP.Setting(CSettingManager::LapisRoof) ){
                    resource = CGood::lapis;
                    resourceAmount = GAP.Setting(CSettingManager::LapisPerBlock);
                }
                if( resource == 0 &&
                   (GAP.ChunkManager.LimeMap()->GetValue(k, i) * GAP.Setting(CSettingManager::LimeRand)) + resourceRand
                   > GAP.Setting(CSettingManager::LimeRoof) ){
                    resource = CGood::lime;
                    resourceAmount = GAP.Setting(CSettingManager::LimePerBlock);
                }
                if( resource == 0 &&
                   (GAP.ChunkManager.MarbleMap()->GetValue(k, i) * GAP.Setting(CSettingManager::MarbleRand)) + resourceRand
                   > GAP.Setting(CSettingManager::MarbleRoof) ){
                    resource = CGood::marble;
                    resourceAmount = GAP.Setting(CSettingManager::MarblePerBlock);
                }
                if(resource != 0){
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

    SDL_Rect destSRect;
    SDL_Rect srcSRect;
    srcSRect.h = CScreen::tileWidth;
    srcSRect.w = CScreen::tileWidth;
    destSRect.h = CScreen::tileWidth;
    destSRect.w = CScreen::tileWidth;
    for(int k = 0; k < tilesPerChunk; k++){
        for(int i = 0; i < tilesPerChunk; i++){
            destSRect.x = CScreen::tileWidth * i;
            destSRect.y = CScreen::tileWidth * k;
            srcSRect.x = Tiles[k][i]->GetClip().x;
            srcSRect.y = Tiles[k][i]->GetClip().y;
            SDL_BlitSurface(sourceSurf,
                    &srcSRect,
                    targetSurf,
                    &destSRect);
        }
    }

    tilesTexture = SDLCALL GPU_CopyImageFromSurface	(targetSurf);

    SDL_FreeSurface(sourceSurf);
    SDL_FreeSurface(targetSurf);

    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = tilesPerChunk * CScreen::tileWidth;
    srcRect.h = tilesPerChunk * CScreen::tileWidth;
    destRect.x = (tilesPerChunk * CScreen::tileWidth) * chunkX;
    destRect.y = (tilesPerChunk * CScreen::tileWidth) * chunkY;
    destRect.w = tilesPerChunk * CScreen::tileWidth;
    destRect.h = tilesPerChunk * CScreen::tileWidth;
    destRect.w++;
    destRect.h++;

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

bool CChunk::RenderChunkTiles(){
    if(!isInited){
        return false;
    }
    GAP.TextureManager.DrawTextureGL(tilesTexture, &srcRect, &destRect);
    return true;
}

bool CChunk::RenderChunkObjects(){
    if(!isInited){
        return false;
    }
    for(int k = 0; k < tilesPerChunk; k++){
        for(int i = 0; i < tilesPerChunk; i++){
            Tiles[k][i]->Render();
        }
    }
    return true;
}


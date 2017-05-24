

#include "CChunk.h"
#include "CGame.h"


extern CGame GAP;

void CChunk::Init(int chunkX, int chunkY, utils::NoiseMap* heightMap, utils::NoiseMap* heightMapForest, utils::NoiseMap* heightMapStones){

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
                resourceRand = rand() % GAP.Setting(CSettingManager::ResourceRand) ;
                if( (heightMapForest->GetValue(k, i) * GAP.Setting(CSettingManager::ForestRand)) + resourceRand 
                   > GAP.Setting(CSettingManager::ForestRoof) ){
                    resource = CGood::wood;
                    resourceAmount = GAP.Setting(CSettingManager::WoodPerTree);
                    moveCost = 3.0f;
                }
                if( (heightMapStones->GetValue(k, i) * GAP.Setting(CSettingManager::StoneRand)) + resourceRand 
                   > GAP.Setting(CSettingManager::StoneRoof) ){
                    resource = CGood::stone;
                    resourceAmount = GAP.Setting(CSettingManager::StonePerBlock);
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


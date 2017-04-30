
#include <vector>
#include "CChunkManager.h"
#include "CGame.h"
#include <noise/noise.h>
#include "noiseutils.h"


extern CGame GAP;

CChunkManager::CChunkManager(){
    lastRenderedX = -999999;
    lastRenderedY = -999999;
}

CChunkManager::~CChunkManager(){
    //dtor
}

void CChunkManager::Init(){
    currentChunkX = 999999;
    currentChunkY = 999999;
    UpdateChunks();
}

void CChunkManager::UpdateChunks(){
    int oldChunkX = currentChunkX;
    int oldChunkY = currentChunkY;
    currentChunkX = GetChunk(GAP.Player->getTileX());
    currentChunkY = GetChunk(GAP.Player->getTileY());

    if(currentChunkX != oldChunkX || currentChunkY != oldChunkY){
        for(int i = currentChunkY - 1; i <= currentChunkY + 1; i++ ){
            for(int k = currentChunkX - 1; k <= currentChunkX + 1; k++ ){
                if(!ChunkExists(k, i)){
                    GenerateChunk(k, i);
                }
            }
        }
    }

    return;
}

void CChunkManager::RenderTiles(){

    if( abs(lastRenderedX - GAP.Player->getTileX()) > 0 || abs(lastRenderedY - GAP.Player->getTileY()) > 0 ){
        renderedTiles.clear();
        int showWidth = (int)((CScreen::screen_half_w/ CScreen::tileWidth) / GAP.ZoomLvl()) + 2;
        renderedTiles = TilesInTileArea(
                                            GAP.Player->getTileX() - showWidth,
                                            GAP.Player->getTileY() - showWidth,
                                            showWidth * 2,
                                            showWidth * 2
                                                     );
    }


    for(auto e : renderedTiles)
    {
        if(auto s = e.lock()){
            s->Render();
        }
    }

    return;
}

void CChunkManager::RenderObjects(){

    CTree tree;
    for(auto e : renderedTiles)
    {
        if(auto s = e.lock()){
            s->RenderUnits();
            if(s->getResource()){
                tree.Set(s->getX(), s->getY(), s->getResource());
                tree.Render();
            }
        }
    }

    return;
}

void CChunkManager::RenderMinimap(){

//    int showWidth =  10;
//    if(GAP.getTick() % 60){
//        SDL_DestroyTexture(GAP.TextureManager.GetTexture("minimap"));
//        SDL_Texture* texture = SDL_CreateTexture
//            (
//            GAP.getRenderer(),
//            SDL_PIXELFORMAT_ARGB8888,
//            SDL_TEXTUREACCESS_STREAMING,
//            showWidth, showWidth
//            );
//
//        std::vector< unsigned char > pixels( showWidth * showWidth * 4, 0 );
//        std::vector<gui_pointer> tiles = TilesInTileArea(
//                                            GAP.Player->getTileX() - (showWidth/2),
//                                            GAP.Player->getTileY() - (showWidth/2),
//                                            showWidth ,
//                                            showWidth);
//
//        unsigned int tileCount = 0;
//        unsigned int offset;
//        SDL_Color color;
//        for(CGUIObject* e : tiles)
//        {
//            color = e->GetMinimapColor();
//            offset = ( showWidth * 4 * (tileCount / showWidth) ) + (tileCount % showWidth) * 4;
//            pixels[ offset + 0 ] = color.b;        // b
//            pixels[ offset + 1 ] = color.g;        // g
//            pixels[ offset + 2 ] = color.r;        // r
//            pixels[ offset + 3 ] = SDL_ALPHA_OPAQUE;    // a
//            tileCount++;
//        }
//
//        SDL_UpdateTexture(
//            texture,
//            NULL,
//            &pixels[0],
//            showWidth * 4);
//        GAP.TextureManager.SetTexture(texture, "minimap");
//    }

}

bool CChunkManager::ChunkExists(int x, int y){
    if(Matrix.count(y)){
        if(Matrix.at(y).count(x)){
            return true;
        }
    }
    return false;
}

void CChunkManager::GenerateChunk(int x, int y){
    float chunkPortion = 10000 * CScreen::tilesPerChunk;
    chunkPortion = chunkPortion / 1000000;
    float chunkY = chunkPortion * y;
    float chunkX = chunkPortion * x;
    CLog::Write("Generating chunk:");
    CLog::Write(chunkX);
    CLog::Write(chunkY);

    utils::NoiseMap heightMap;
    noise::module::Perlin myModule;
    myModule.SetSeed(GAP.GetSeed());
    utils::NoiseMapBuilderPlane heightMapBuilder;
    heightMapBuilder.SetSourceModule (myModule);
    heightMapBuilder.SetDestNoiseMap (heightMap);
    heightMapBuilder.SetDestSize (CScreen::tilesPerChunk, CScreen::tilesPerChunk);
    heightMapBuilder.SetBounds (chunkY, chunkY + chunkPortion,chunkX, chunkX + chunkPortion);
    heightMapBuilder.Build ();


    Matrix[y][x] = CChunk();
    Matrix.at(y).at(x).Init(x, y, &heightMap);
}

int CChunkManager::getX(){
    return currentChunkX;
}

int CChunkManager::getY(){
    return currentChunkY;
}

std::vector<tile_weak_ptr> CChunkManager::TilesInTileArea(int tileX, int tileY, int tileW, int tileH){
    std::vector<tile_weak_ptr> tiles;
    for(int k = tileY; k < (tileY + tileH); k++){
        for(int i = tileX; i < (tileX + tileW); i++){
            tiles.push_back(GetTile(i,k));
        }
    }

    return tiles;
}

std::vector<tile_weak_ptr> CChunkManager::GetResources(int resourceType, int resourceRadius, int centerX, int centerY){
    std::vector<tile_weak_ptr> tiles = TilesInTileArea(centerX - 1, centerY - 1, 3, 3);
    std::vector<tile_weak_ptr> resources;
    for(auto e : tiles){
        if(auto s = e.lock()){
            if(s->getResource() == resourceType){
                resources.push_back(e);
            }
        }
    }
    if(resources.size() < 1){
        tiles = TilesInTileArea(centerX - resourceRadius, centerY - resourceRadius, (resourceRadius * 2) + 1, (resourceRadius * 2) + 1);
        for(auto e : tiles){
            if(auto s = e.lock()){
                if(s->getResource() == resourceType){
                    if(s->getTileFlightRoundDistance(centerX, centerY) <= resourceRadius){
                        resources.push_back(e);
                    }
                }
            }
        }
    }
    return resources;
}

tile_weak_ptr CChunkManager::GetTile(int x, int y){
    if(!ChunkExists(GetChunk(x), GetChunk(y))){
        GenerateChunk(GetChunk(x),GetChunk(y));
    }
    return Matrix
                .at(GetChunk(y))
                .at(GetChunk(x))
                .GetTile(x,y);
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
    tile_weak_ptr optr = Matrix.at(ocy).at(ocx).GetTile(x,y);
    tile_weak_ptr nptr = Matrix.at(ncy).at(ncx).GetTile(nx,ny);
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
    Matrix.at(ocy).at(ocx).RemoveUnit(id);
    if(!removeUnit){
        Matrix.at(ocy).at(ocx).AddUnit(ptr);
    }
}

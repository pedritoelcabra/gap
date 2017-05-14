#ifndef CCHUNKMANAGER_H
#define CCHUNKMANAGER_H

#include <map>
#include <unordered_map>
#include "CChunk.h"
#include "CGuiObject.h"
#include "CScreen.h"

typedef std::shared_ptr<CGUIObject> gui_shared_ptr;
typedef std::weak_ptr<CGUIObject> gui_weak_ptr;
typedef std::shared_ptr<CChunk> chunk_shared_ptr;
typedef std::weak_ptr<CChunk> chunk_weak_ptr;
typedef std::unordered_map< int, std::unordered_map< int, chunk_shared_ptr > > matrix_type;

class CChunkManager
{
    public:
        CChunkManager(){};
        virtual ~CChunkManager(){};

        void                        UpdateChunks(bool forceUpdate = false);
        void                        RenderTiles();
        void                        RenderObjects();
        void                        RenderMinimap();
        void                        Init();
        tile_weak_ptr               GetTile(int x, int y);
        int                         GetChunk(int tile);
        std::vector<tile_weak_ptr>  GetResources(int resourceType, int resourceRadius, int tileX, int tileY);
        void                        MoveUnit(int x, int y, int nx, int ny, unit_weak_ptr ptr, int id, bool removeUnit = false);

        int                         GetX(){             return currentChunkX;};
        int                         GetY(){             return currentChunkY;};
    protected:
    private:
        int                 currentChunkX;
        int                 currentChunkY;
        matrix_type         Matrix;
        chunk_shared_ptr    GenericChunk;
        bool                ChunkExists(int x, int y);
        bool                GenerateChunk(int x, int y);

        const static int    tilesPerChunk = CScreen::tilesPerChunk;
        const static int    chunkWidth = tilesPerChunk * CScreen::tileWidth;

        void                            TilesInTileArea(std::vector<tile_weak_ptr>* tileVec, int x, int y, int w, int h);
        std::vector<tile_weak_ptr>        renderedTiles;


        utils::NoiseMapBuilderPlane             heightMapBuilder;
        utils::NoiseMap                         heightMap;
        noise::module::Perlin                   myModule;
        utils::NoiseMap                         heightMapForest;
        noise::module::Perlin                   myModuleForest;
        utils::NoiseMap                         heightMapStones;
        noise::module::Perlin                   myModuleStones;

        float                                   chunkPortion;

        int                                     xOff = 0;
        int                                     yOff = 0;
        int                                     showWidth;
        int                                     showHeight;
        int                                     topY;
        int                                     botY;
        int                                     leftX;
        int                                     rightX;
};

#endif // CCHUNKMANAGER_H

#ifndef CCHUNK_H
#define CCHUNK_H

#include <memory>
#include <map>
#include <unordered_map>
#include <vector>
#include <ctime>
#include <stdexcept>
#include "CTile.h"
#include <noise/noise.h>
#include "noiseutils.h"
#include "CGuiObject.h"
#include "CScreen.h"

typedef std::shared_ptr<CTile> tile_shared_ptr;
typedef std::weak_ptr<CTile> tile_weak_ptr;

typedef std::unordered_map<int, std::unordered_map<int, tile_shared_ptr>> tilemap_type;
typedef tile_shared_ptr tile_array[CScreen::tilesPerChunk][CScreen::tilesPerChunk];


class CChunk
{
    public:
        CChunk(){};
        virtual ~CChunk(){};

        void                            Init(int x, int y);
        tile_weak_ptr                   GetTile(int x, int y);
        void                            AddUnit(unit_weak_ptr ptr);
        void                            RemoveUnit(int id);
        bool                            IsInited(){ return isInited; };
        bool                            RenderChunkTiles();
        bool                            RenderChunkObjects();
    protected:
    private:
        tile_array                      Tiles;
        int                             cx;
        int                             offX;
        int                             cy;
        int                             offY;
        const static int                tilesPerChunk = CScreen::tilesPerChunk;
        std::vector<unit_weak_ptr>      Units;
        bool                            isInited = false;
        GPU_Image*                      tilesTexture;
        GPU_Rect                        srcRect;
        GPU_Rect                        destRect;
};

#endif // CCHUNK_H

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

class CChunk
{
    public:
        CChunk(){};
        virtual ~CChunk(){};

        void                            Init(int x, int y, utils::NoiseMap* heightMap, utils::NoiseMap* heightMapForest, utils::NoiseMap* heightMapStones);
        tile_weak_ptr                   GetTile(int x, int y);
        void                            AddUnit(unit_weak_ptr ptr);
        void                            RemoveUnit(int id);
    protected:
    private:
        tilemap_type                    Tiles;
        int     cx;
        int     cy;
        const static int                tilesPerChunk = CScreen::tilesPerChunk;
        std::vector<unit_weak_ptr>      Units;
};

#endif // CCHUNK_H

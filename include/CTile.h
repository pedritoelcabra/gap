#ifndef CTILE_H
#define CTILE_H

#include <memory>
#include <CGuiObject.h>
#include <CUnit.h>
#include "CTree.h"

class CUnit;
class CTile;

typedef std::weak_ptr<CUnit> unit_weak_ptr;
typedef std::unique_ptr<CTree> tree_uniq_ptr;
typedef std::weak_ptr<CTile> tile_weak_ptr;
typedef std::shared_ptr<CTile> tile_shared_ptr;

class CTile : public CGUIObject
{
    public:
        CTile(){};
        virtual ~CTile(){};
        void                        SetTerrain(int terrain);
        bool                        Render(int row = 0, int col = 0);
        bool                        RenderUnits();
        bool                        RenderResource();
        bool                        RenderOnMinimap(int x, int y);
        SDL_Color                   GetMinimapColor();
        void                        Init(int type, int tx, int ty, int tree_, int resourceAmount_, int resourceVariety_);
        void                        AddUnit(unit_weak_ptr ptr);
        void                        RemoveUnit(int id);
        std::vector<unit_weak_ptr>  UnitsAtTile();
        int                         HarvestResource(int amount = 1);

        int                         Owner(){                    return owner;};
        float                       GetMoveCost(){              return moveCost;};
        int                         GetResource(){              return resource;};
        int                         GetResourceAmount(){        return resourceAmount;};
        int                         GetVariety(){               return resourceVariety;};
        void                        SetMoveCost(float cost_){   moveCost = cost_;};
        void                        SetRightNeighbour(tile_weak_ptr rightNeighbour_){ rightNeighbour = rightNeighbour_.lock(); };
        void                        SetDownNeighbour(tile_weak_ptr downNeighbour_){ downNeighbour = downNeighbour_.lock(); };

        static std::string  spriteSheet;
    protected:
    private:
        int                 cx;
        int                 cy;
        int                 sheetX;
        int                 sheetY;
        int                 terrain;
        int                 resource;
        int                 resourceAmount;
        int                 resourceVariety;
        tree_uniq_ptr       resourcePtr;
        float               moveCost = CScreen::flatMoveCost;

        tile_shared_ptr     rightNeighbour;
        tile_shared_ptr     downNeighbour;

        static const int    tileWidth = CScreen::tileWidth;

        std::vector<unit_weak_ptr>          Units;
        int                 owner = 0;
};

#endif // CTILE_H

#ifndef CTILE_H
#define CTILE_H

#include <CGuiObject.h>
#include <CUnit.h>

class CUnit;

typedef std::weak_ptr<CUnit> unit_weak_ptr;

class CTile : public CGUIObject
{
    public:
        CTile();
        virtual ~CTile();
        void                        SetTerrain(int terrain);
        bool                        Render();
        bool                        RenderUnits();
        bool                        RenderOnMinimap(int x, int y);
        SDL_Color                   GetMinimapColor();
        void                        Init(int type, int tx, int ty, int tree_, int resourceAmount_, int resourceVariety_);
        void                        AddUnit(unit_weak_ptr ptr);
        void                        RemoveUnit(int id);
        std::vector<unit_weak_ptr>  UnitsAtTile();
        int                         HarvestResource();

        float                       GetMoveCost(){              return moveCost;};
        int                         GetResource(){              return resource;};
        int                         GetVariety(){               return resourceVariety;};
        void                        SetMoveCost(float cost_){   moveCost = cost_;};
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
        float               moveCost;

        static const int    tileWidth = CScreen::tileWidth;

        std::vector<unit_weak_ptr>          Units;
};

#endif // CTILE_H

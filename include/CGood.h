#ifndef CGOOD_H
#define CGOOD_H

#include <map>
#include "CGuiObject.h"

class CGood : public CGUIObject
{
    public:
        enum EResources { nothing, wood, stone, work, plank, wheat, ironOre, copperOre, tinOre, coal, goldOre, silverOre,
            sand, marble, gemstones, fish, lime, clay, lapis, salt, ebony, hops, grapes, fruit, herbs, cotton, wool, meat, bones,
            hides, horse, donkey, gallnuts, gold, silver, tin, copper, bronze, iron, pigIron, steel, leather, ink, parchment, fabric,
            gems, glass, planks, basicInstruments, advancedInstruments, beer, books, wine, bricks, mortar, reinforcedDoor,
            rudeClothes, basicFurniture, earthenware, simpleFood, elegantClothes, nobleFurniture, ornatePorcelain, elaborateFood,
            paddedArmor, woodenShield, woodenClub, woodenSpear, woodenBow, stoneheadArrows, leatherArmor, leatherHelmet, bronzeShield,
            bronzeGladius, bronzeSpear, bronzeheadArrows, ironChainmail, ironHelmet, ironCuirass, ironShield, ironSword, ironSpear,
            compositeBow, ironheadArrows, steelChainmail, steelShield, steelCuirass, steelHelmet, steelSword, steelSpear, steelheadArrows,
            pig, bruteResearch, peasantResearch, citizenResearch, noblemenResearch };

        static const std::map<int, std::string >    GetResources();
        static const std::string                    GetResourceName(int resource);
        static const int                            GetResourceByName(std::string resourceName);
        static const int                            GetResourceIcon(int resource);
        static bool                                 Render(GPU_Rect* box, int resource, int amount = 0, bool absolutePos = false);

    protected:

    private:
        static std::map<int, std::string >          ResourceNames;
        static std::map<int, int >                  ResourceIcons;
        static  std::string                         iconSpriteName;
};

#endif // CGOOD_H  zzzzzzzzz

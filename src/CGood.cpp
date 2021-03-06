#include "CGood.h"
#include "CGame.h"

extern CGame GAP;
std::string CGood::iconSpriteName = "icons";

std::map<int, std::string > CGood::ResourceNames = {
   {CGood::wood, "Wood"},
   {CGood::stone, "Stone"},
   {CGood::work, "Work"},
   {CGood::plank, "Plank"},
   {CGood::wheat, "Wheat"},
   {CGood::ironOre, "Iron Ore" },
   {CGood::copperOre, "Copper Ore"},
   {CGood::tinOre, "Tin Ore"},
   {CGood::coal, "Coal"},
   {CGood::goldOre, "Gold Ore"},
   {CGood::silverOre, "Silver Ore"},
   {CGood::sand, "Sand"},
   {CGood::marble, "Marble"},
   {CGood::gemstones, "Gemstones"},
   {CGood::fish, "Fish"},
   {CGood::lime, "Lime"},
   {CGood::clay, "Clay"},
   {CGood::lapis, "Lapis Lazuli"},
   {CGood::salt, "Salt"},
   {CGood::ebony, "Ebony"},
   {CGood::hops, "Hops"},
   {CGood::grapes, "Grapes"},
   {CGood::fruit, "Fruit"},
   {CGood::herbs, "Herbs"},
   {CGood::cotton, "Cotton"},
   {CGood::wool, "Wool"},
   {CGood::meat, "Meat"},
   {CGood::bones, "Bones"},
   {CGood::hides, "Hides"},
   {CGood::horse, "Horse"},
   {CGood::donkey, "Donkey"},
   {CGood::gallnuts, "Gallnuts"},
   {CGood::gold, "Gold"},
   {CGood::silver, "Silver"},
   {CGood::tin, "Tin"},
   {CGood::copper, "Copper"},
   {CGood::bronze, "Bronze"},
   {CGood::iron, "Iron"},
   {CGood::pigIron, "Pig Iron"},
   {CGood::steel, "Steel"},
   {CGood::leather, "Leather"},
   {CGood::ink, "Ink"},
   {CGood::parchment, "Parchment"},
   {CGood::fabric, "Fabric"},
   {CGood::gems, "Gems"},
   {CGood::glass, "Glass"},
   {CGood::basicInstruments, "Basic Instrument"},
   {CGood::advancedInstruments, "Advanced Instrument"},
   {CGood::beer, "Beer"},
   {CGood::books, "Books"},
   {CGood::wine, "Wine"},
   {CGood::bricks, "Bricks"},
   {CGood::mortar, "Mortar"},
   {CGood::reinforcedDoor, "Reinforced Door"},
   {CGood::basicFurniture, "Basic Furniture"},
   {CGood::earthenware, "Earthenware"},
   {CGood::simpleFood, "Simple Food"},
   {CGood::elegantClothes, "Elegant Clothes"},
   {CGood::rudeClothes, "Rude Clothes"},
   {CGood::earthenware, "Earthenware"},
   {CGood::nobleFurniture, "Noble Furniture"},
   {CGood::ornatePorcelain, "Ornate Porcelain"},
   {CGood::elaborateFood, "Elaborate Food"},
   {CGood::paddedArmor, "Padded Armor"},
   {CGood::woodenBow, "Wooden Bow"},
   {CGood::woodenClub, "Wooden Club"},
   {CGood::woodenShield, "Wooden Shield"},
   {CGood::woodenSpear, "Wooden Spear"},
   {CGood::stoneheadArrows, "Stonehead Arrows"},
   {CGood::leatherArmor, "Leather Armor"},
   {CGood::leatherHelmet, "Leather Helmet"},
   {CGood::bronzeGladius, "Bronze Gladius"},
   {CGood::bronzeheadArrows, "Bronzehead Arrows"},
   {CGood::bronzeShield, "Bronze Shield"},
   {CGood::bronzeSpear, "Bronze Spear"},
   {CGood::ironChainmail, "Iron Chainmail"},
   {CGood::ironCuirass, "Iron Cuirass"},
   {CGood::ironHelmet, "Iron Helmet"},
   {CGood::ironShield, "Iron Shield"},
   {CGood::ironSpear, "Iron Spear"},
   {CGood::ironSword, "Iron Sword"},
   {CGood::ironheadArrows, "Ironhead Arrows"},
   {CGood::steelChainmail, "Steel Chainmail"},
   {CGood::steelCuirass, "Steel Cuirass"},
   {CGood::steelheadArrows, "Steelhead Arrows"},
   {CGood::steelHelmet, "Steel Helmet"},
   {CGood::steelShield, "Steel Shield"},
   {CGood::steelSpear, "Steel Spear"},
   {CGood::steelSword, "Steel Sword"},
   {CGood::pig, "Pig"},
   {CGood::flour, "Flour"},
   {CGood::compositeBow, "Composite Bow"},
   {CGood::bruteResearch, "Brute Research"},
   {CGood::peasantResearch, "Peasant Research"},
   {CGood::citizenResearch, "Citizen Research"},
   {CGood::noblemenResearch, "Noblemen Research"}
};

std::map<int, int > CGood::ResourceIcons = {
   {CGood::wood, 3051},
   {CGood::stone, 2996},
   {CGood::work, 1},
   {CGood::plank, 3052},
   {CGood::wheat, 3053},
   {CGood::ironOre, 3072},
   {CGood::copperOre, 3067},
   {CGood::tinOre, 3071},
   {CGood::coal, 3068},
   {CGood::goldOre, 3069},
   {CGood::silverOre, 3070},
   {CGood::sand, 3117},
   {CGood::marble, 3112},
   {CGood::gemstones, 3073},
   {CGood::fish, 3065},
   {CGood::lime, 3115},
   {CGood::clay, 3113},
   {CGood::lapis, 3114},
   {CGood::salt, 3116},
   {CGood::ebony, 3085},
   {CGood::hops, 3059},
   {CGood::grapes, 3061},
   {CGood::fruit, 3062},
   {CGood::herbs, 3060},
   {CGood::cotton, 3118},
   {CGood::wool, 3119},
   {CGood::meat, 3064},
   {CGood::bones, 3122},
   {CGood::hides, 3087},
   {CGood::horse, 3134},
   {CGood::donkey, 3135},
   {CGood::gallnuts, 3120},
   {CGood::gold, 3078},
   {CGood::silver, 3077},
   {CGood::tin, 3080},
   {CGood::copper, 3079},
   {CGood::bronze, 3082},
   {CGood::iron, 3083},
   {CGood::pigIron, 3081},
   {CGood::steel, 3084},
   {CGood::leather, 3086},
   {CGood::ink, 3126},
   {CGood::parchment, 3066},
   {CGood::fabric, 3127},
   {CGood::gems, 3074},
   {CGood::glass, 3125},
   {CGood::basicInstruments, 3123},
   {CGood::advancedInstruments, 3124},
   {CGood::beer, 3076},
   {CGood::books, 3121},
   {CGood::wine, 3075},
   {CGood::bricks, 3050},
   {CGood::mortar, 3128},
   {CGood::reinforcedDoor, 3049},
   {CGood::rudeClothes, 3057},
   {CGood::basicFurniture, 3129},
   {CGood::earthenware, 3132},
   {CGood::simpleFood, 3063},
   {CGood::elegantClothes, 3058},
   {CGood::nobleFurniture, 3130},
   {CGood::ornatePorcelain, 3133},
   {CGood::elaborateFood, 3131},
   {CGood::paddedArmor, 3108},
   {CGood::woodenBow, 3088},
   {CGood::woodenClub, 3056},
   {CGood::woodenShield, 3102},
   {CGood::woodenSpear, 3098},
   {CGood::stoneheadArrows, 3092},
   {CGood::leatherArmor, 3109},
   {CGood::leatherHelmet, 3099},
   {CGood::bronzeGladius, 3094},
   {CGood::bronzeheadArrows, 3093},
   {CGood::bronzeShield, 3103},
   {CGood::bronzeSpear, 3096},
   {CGood::ironChainmail, 3106},
   {CGood::ironCuirass, 3110},
   {CGood::ironHelmet, 3100},
   {CGood::ironShield, 3104},
   {CGood::ironSpear, 3055},
   {CGood::ironSword, 3095},
   {CGood::ironheadArrows, 3091},
   {CGood::steelChainmail, 3107},
   {CGood::steelCuirass, 3111},
   {CGood::steelheadArrows, 3090},
   {CGood::steelHelmet, 3101},
   {CGood::steelShield, 3105},
   {CGood::steelSpear, 3097},
   {CGood::steelSword, 3054},
   {CGood::pig, 2689},
   {CGood::flour, 3048},
   {CGood::compositeBow, 3089}
};

const std::map<int, std::string > CGood::GetResources(){
    return ResourceNames;
}

const std::string CGood::GetResourceName(int resource){
    return ResourceNames.at(resource);
}

const int CGood::GetResourceByName(std::string resourceName){
    for (auto it = ResourceNames.begin(); it != ResourceNames.end(); ++it ){
        if (!resourceName.compare(it->second)){
            return it->first;
        }
    }
    CLog::Write("Couldn't load resource:");
    CLog::Write(resourceName);
    return 0;
}

const int CGood::GetResourceIcon(int resource){
    return ResourceIcons.at(resource);
}

bool CGood::Render(GPU_Rect* box, int resource, int amount, bool absolutePos){
    GAP.TextureManager.DrawTextureGL(&iconSpriteName, GAP.TextureManager.GetIconClip32(GetResourceIcon(resource)), box, absolutePos);
    if(amount){
        SDL_Color black = {0, 0, 0};
        SDL_Surface* textSurface = TTF_RenderText_Solid( GAP.TextureManager.GetFont(16), std::to_string(amount).c_str(), black );
        GPU_Image* texture = GPU_CopyImageFromSurface( textSurface );
        GPU_Rect Message_rect; //create a rect
        Message_rect.x = box->x + box->w;  //controls the rect's x coordinate
        Message_rect.y = box->y; // controls the rect's y coordinte
        Message_rect.w = box->w; // controls the width of the rect
        if(amount >= 10){
            Message_rect.w += box->w;
        }
        if(amount >= 100){
            Message_rect.w += box->w;
        }
        Message_rect.h = box->h; // controls the height of the rect

        GAP.TextureManager.DrawTextureGL(texture, 0, &Message_rect, true);
        GPU_FreeImage(texture);
        SDL_FreeSurface( textSurface );
    }
    return true;
}



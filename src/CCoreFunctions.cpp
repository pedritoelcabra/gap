#include "CGame.h"
#include "CEnemy.h"
#include "CPlayer.h"

bool CGame::OnInit(){
    CLog::Init();
    CLog::Write("Initializing...");
    zoom = 1;

    LoadSettings();
    SettingsManager.Init();

    SDL_SetHint(SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING, "1");


    if (SDL_Init(SDL_INIT_EVERYTHING
                 | SDL_INIT_AUDIO) < 0){
        CLog::Write("SDL could not initialize!");
        return false;
    }

    InitGL();

    Player = std::make_shared<CPlayer>(CScreen::tilesPerChunk / 2, CScreen::tilesPerChunk / 2 , "sky_god");
    Player->Owner(1);

    ChunkManager.Init();
    TechManager.Init();
    RecipeManager.Init();
    BuildingManager.Init();


    UnitManager.AddNPC(Player);

    MenuManager.BuildMenus();

    for(int y = 0; y < CScreen::tilesPerChunk; y++){
        for(int x = 0; x < CScreen::tilesPerChunk; x++){
            tile_weak_ptr tile = ChunkManager.GetTile(x,y);
            if(auto s = tile.lock()){
                s->HarvestResource(1000);
            }
        }
    }

    auto buildingTypes = BuildingManager.GetBuildingTypes();
    int buildingCount = 0;
    int chiefW = 8, chiefH = 8;
    for(auto bType : buildingTypes){
        if(bType.DistributionRange()){
            auto w = BuildingManager.AddBuilding(buildingCount,
                                        Player->GetTileX() - (chiefW/2) + 1,
                                        Player->GetTileY() - chiefH ,
                                        1);
            if(auto s = w.lock()){
                s->AddToInventory(CGood::wood, 1);
                s->AddWork(1000);
                s->AddToInventory(CGood::wood, 20);
                s->AddToInventory(CGood::stone, 20);
//                for(auto r : CGood::GetResources()){
//                    if(1){// r.first ==
//                        s->AddToInventory(r.first, 99);
//                    }
//                }
            }
        }
        buildingCount++;
    }



    return true;
}

bool CGame::InitGL(){

    MainViewport = { 0, 0, CScreen::screen_w, CScreen::screen_h };
    GPU_SetDebugLevel(GPU_DEBUG_LEVEL_MAX);
    MainGLWindow = GPU_Init(CScreen::screen_w, CScreen::screen_h, GPU_DEFAULT_INIT_FLAGS);
	if(MainGLWindow == NULL){
		return -1;
	}
    TextureManager.Init();

    TextureManager.LoadTextureGL("background", "gfx/BGload.png");

    TextureManager.LoadTextureGL("player", "gfx/chars/1.png");
    TextureManager.LoadTextureGL("sky_god", "gfx/chars/sky_god.png");
    TextureManager.LoadTextureGL("forest_god", "gfx/chars/forest_god.png");
    TextureManager.LoadTextureGL("death_god", "gfx/chars/death_god.png");
    TextureManager.LoadTextureGL("brute", "gfx/chars/brute.png");
    TextureManager.LoadTextureGL("peasant", "gfx/chars/peasant.png");
    TextureManager.LoadTextureGL("citizen", "gfx/chars/citizen.png");
    TextureManager.LoadTextureGL("nobleman", "gfx/chars/nobleman.png");
    TextureManager.LoadTextureGL("trader", "gfx/chars/trader.png");
    TextureManager.LoadTextureGL("orc", "gfx/chars/orc.png");

    TextureManager.LoadTextureGL("tiles", "gfx/tile_sprites.png");
    TextureManager.LoadTextureGL("menusprites", "gfx/menusprites.png");
    TextureManager.LoadTextureGL("icons", "gfx/icons.png");
    TextureManager.LoadTextureGL("minimap", "gfx/minimap.png");

    TextureManager.LoadTextureGL("tree", "gfx/resources/tree.png");
    TextureManager.LoadTextureGL("rock", "gfx/resources/rock.png");
    TextureManager.LoadTextureGL("copper", "gfx/resources/copper.png");
    TextureManager.LoadTextureGL("tin", "gfx/resources/tin.png");
    TextureManager.LoadTextureGL("coal", "gfx/resources/coal.png");
    TextureManager.LoadTextureGL("iron", "gfx/resources/iron.png");
    TextureManager.LoadTextureGL("silver", "gfx/resources/silver.png");
    TextureManager.LoadTextureGL("gold", "gfx/resources/gold.png");
    TextureManager.LoadTextureGL("gems", "gfx/resources/gems.png");
    TextureManager.LoadTextureGL("lapis", "gfx/resources/lapis.png");
    TextureManager.LoadTextureGL("lime", "gfx/resources/lime.png");
    TextureManager.LoadTextureGL("marble", "gfx/resources/marble.png");
    TextureManager.LoadTextureGL("ebony", "gfx/resources/ebony.png");

    if( TTF_Init() == -1 )
    {
        CLog::Write( "SDL_ttf could not initialize!");
        return 1;
    }

    TextureManager.LoadFont("gfx/fonts/DroidSans-Bold.ttf");

    MainCamera.zoom = 1.0f;
    return true;
}

void CGame::OnCleanUp(){
    SaveSettings();
	SDL_GL_DeleteContext(MainContext);
	SDL_Quit();
    return;
}

bool CGame::LoadSettings(){

    Settings.emplace("Music", 1);
    Settings.emplace("Sound", 1);
    Settings.emplace("FullScreen", 1);
    Settings.emplace("ShowDebug", 1);

    std::ifstream load( "settings.txt" );
    int val;
    std::string var;
    while (load >> var >> val)
    {
        Settings[var] = val;
    }
    load.close();
    return 1;
}

bool CGame::SaveSettings(){

    std::ofstream save;
    save.open("settings.txt");
    for (auto const& i : Settings)
    {
        save << i.first << " " << i.second << " \n";
    }
    save.close();
    return 1;
}

void CGame::Exit(){
    running = false;
}

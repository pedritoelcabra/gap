
#include "CGame.h"

bool CGame::OnInit(){
    CLog::Init();
    CLog::Write("Initializing...");
    zoom = 1;

    LoadSettings();

    SDL_SetHint(SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING, "1");


    if (SDL_Init(SDL_INIT_EVERYTHING
                 | SDL_INIT_AUDIO) < 0){
        CLog::Write("SDL could not initialize!");
        return false;
    }

    InitGL();

    Player = std::make_shared<CUnit>(CScreen::tilesPerChunk / 2, CScreen::tilesPerChunk / 2 , "sky_god");

    ChunkManager.Init();
    BuildingManager.Init();


    UnitManager.AddNPC(Player);

    MenuManager.BuildMenus();

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
    TextureManager.LoadTextureGL("tiles", "gfx/tile_sprites.png");
    TextureManager.LoadTextureGL("menusprites", "gfx/menusprites.png");
    TextureManager.LoadTextureGL("minimap", "gfx/minimap.png");
    TextureManager.LoadTextureGL("tree", "gfx/resources/tree.png");
    TextureManager.LoadTextureGL("rock", "gfx/resources/rock.png");

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

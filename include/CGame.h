/// GAP - Main header

#ifndef _CGAME_H
    #define _CGAME_H

#include <thread>
#include <chrono>
#include <map>
#include <cstring>
#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_gpu.h"
#define GLEW_STATIC
#include "gl/glew.h"
#include <SDL2/SDL_opengl.h>
#include "CEvent.h"
#include "CLog.h"
#include "CScreen.h"
#include "CSurface.h"
#include "CGuiObject.h"
#include "CCharacter.h"
#include "CTextureManager.h"
#include "CChunkManager.h"
#include "CMenuManager.h"
#include "CUnitManager.h"
#include "CBuildingManager.h"
#include "CPathfinder.h"
#include "CTree.h"

typedef std::map<std::string, int> StrIntMap;

typedef CGUIObject* gui_pointer;

class CGame : public CEvent{

public:
    GPU_Target*         MainGLWindow;
    SDL_GLContext       MainContext;
    SDL_Renderer*       TheScreenRen;
    SDL_Rect            MainViewport;
    unit_shared_ptr     Player;
    GPU_Camera          MainCamera;

    CTextureManager     TextureManager;
    CChunkManager       ChunkManager;
    CMenuManager        MenuManager;
    CUnitManager        UnitManager;
    CBuildingManager    BuildingManager;
    CPathfinder         Pathfinder;


    CGame();
    int                 OnExecute();
    bool                OnInit();
    bool                InitGL();
    void                OnEvent(SDL_Event* event);
    void                OnRenderGL();
    void                OnProcessTick();
    void                OnCleanUp();
    bool                LoadSettings();
    void                SetDefines();
    void                SetViewPort(float x, float y);
    bool                SaveSettings();
    void                OnKeyDown(SDL_Keycode key);
    void                OnKeyUp(SDL_Keycode key);
    bool                OnMouseWheel(bool Up, int x, int y);
    bool                OnMouseMove(int x, int y);
    bool                OnLButtonDown(int x, int y);
    bool                OnRButtonDown(int x, int y);
    void                LoadMainMenu();
    int                 TileToPixel(int tile);
    int                 PixelToTile(int pixel);
    void                Exit();

    int                 TranslateXWtoS(int x);
    int                 TranslateXStoW(int x);
    int                 TranslateYWtoS(int y);
    int                 TranslateYStoW(int y);

    int                 GetSeed() const {return seed;};
    Uint32              GetTick(){return tick;};
    int                 GetMouseTileX(){return mouseTileX;};
    int                 GetMouseTileY(){return mouseTileY;};
    float               ZoomLvl(){return zoom;};

    build_weak_ptr                          Building(int id);
    build_weak_ptr                          BuildingAt(int x, int y);
    CBuildingType*                          BuildingType(int type);
    tile_weak_ptr                           Tile(int x, int y);
    unit_weak_ptr                           Unit(int target);
    std::vector<unit_weak_ptr>              UnitsAtTile(int x, int y);

    TTF_Font*           gameFont;


    const int            moveSpeed = 20;
    const int            moveSpeedD = 20;
    const Uint32         frameCap = 60;
    const Uint32         frameMS = 1000/frameCap;

private:

    StrIntMap           Settings;

    void                ShowDebugInfo();
    GLuint              LoadShaders();
    void                UpdatePlayerPosition();

    void                DisplayText(const char* text, int yoff);
    float               zoom;
    int                 halfW;
    int                 halfH;
    int                 mouseX;
    int                 mouseTileX;
    int                 mouseY;
    int                 mouseTileY;
    int                 frameCount = 0;
    Uint32              tick = 0;
    int                 framesUntilLastSecond = 0;
    int                 currentSecond = 0;
    int                 fps = 0;
    bool                running = true;
    bool                paused = false;
    int                 seed;


    GLuint              gProgramID;
    GLint               gVertexPos2DLocation;
    GLuint              gVBO;
    GLuint              gIBO;

};

#endif // CGAME_H

/// GAP -

#include "CGame.h"

void CGame::OnEvent(SDL_Event* event){

    CEvent::OnEvent(event);
}

SDL_Renderer* CGame::getRenderer(){
    return TheScreenRen;
}

void CGame::OnRenderGL(){
    GPU_Clear(MainGLWindow);

    ChunkManager.RenderTiles();
    BuildingManager.Render();
    ChunkManager.RenderObjects();
    MenuManager.Render();

    if(Settings.at("ShowDebug")){
         ShowDebugInfo();
    }

    GPU_Flip(MainGLWindow);
    return;
}

void CGame::ShowDebugInfo(){


    std::stringstream tmp;
    tmp << "PlayerX: " << Player->getX() << " PlayerY: " << Player->getY();
    DisplayText(tmp.str().c_str(), 0);
    tmp.str(std::string());
    tmp << "ChunkX: " << ChunkManager.getX() << " ChunkY: " << ChunkManager.getY();
    DisplayText(tmp.str().c_str(), 15);
    tmp.str(std::string());
    tmp << "Zoom: " << zoom;
    DisplayText(tmp.str().c_str(), 30);
    tmp.str(std::string());
    tmp << "Viewport X: " << MainViewport.x << "     Y: " << MainViewport.y;
    DisplayText(tmp.str().c_str(), 45);
    tmp.str(std::string());
    tmp << "Mouse X: " << mouseX << "     Y: " << mouseY;
    DisplayText(tmp.str().c_str(), 60);
    tmp.str(std::string());
    tmp << "FPS: " << fps;
    DisplayText(tmp.str().c_str(), 75);
    tmp.str(std::string());
    tmp << "Tick: " << tick;
    DisplayText(tmp.str().c_str(), 90);
    tmp.str(std::string());
    tmp << "Player TileX: " << Player->getTileX() << " Player TileY: " << Player->getTileY();
    DisplayText(tmp.str().c_str(), 105);
    tmp.str(std::string());
    tmp << "Mouse Tile X: " << mouseTileX << "     Y: " << mouseTileY;
    DisplayText(tmp.str().c_str(), 120);

}

void CGame::DisplayText(const char* text, int yoff){

    SDL_Color White = {255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid( TextureManager.GetFont(16), text, White );
    GPU_Image* texture = GPU_CopyImageFromSurface( textSurface );
    GPU_Rect Message_rect; //create a rect
    Message_rect.x = 0;  //controls the rect's x coordinate
    Message_rect.y = yoff + 600; // controls the rect's y coordinte
    Message_rect.w = strlen(text)*8; // controls the width of the rect
    Message_rect.h = 15; // controls the height of the rect

    TextureManager.DrawTextureGL(texture, 0, &Message_rect, true);
    GPU_FreeImage(texture);
    SDL_FreeSurface( textSurface );

}

void CGame::OnProcessTick(){

    UpdatePlayerPosition();

    mouseTileX = floor(( (mouseX/zoom) + MainViewport.x ) / CScreen::tileWidth);
    mouseTileY = floor(( (mouseY/zoom) + MainViewport.y ) / CScreen::tileWidth);


    ChunkManager.UpdateChunks();
    BuildingManager.Update();
    UnitManager.Update();
    tick++;
}

float CGame::ZoomLvl(){

    return zoom;
}

int CGame::getMouseTileX(){
    return mouseTileX;
}

int CGame::getMouseTileY(){
    return mouseTileY;
}

int CGame::tileToPixel(int tile){
    return CScreen::tileWidth * tile;
}

int CGame::pixelToTile(int pixel){
    return pixel / CScreen::tileWidth;
}

int CGame::TranslateXWtoS(int x){
    return x;
}

int CGame::TranslateXStoW(int x){
    return (x / zoom ) + MainViewport.x;
}

int CGame::TranslateYWtoS(int y){
    return y;
}

int CGame::TranslateYStoW(int y){
    return (y / zoom ) + MainViewport.y;
}

Uint32 CGame::getTick(){
    return tick;
}

void CGame::SetViewPort(float x, float y){
    MainViewport.x = x;
    MainViewport.y = y;
}

bool CGame::OnMouseWheel(bool Up, int x, int y){
    if (Up){
        if(zoom < 3){
            zoom = zoom + 0.2;
        }
    }else{
        if(zoom > 0.6){
            zoom = zoom - 0.2;
        }
    }

    return true;
}

bool CGame::OnMouseMove(int x, int y){
    mouseX = x;
    mouseY = y;
    return true;
}

void CGame::OnKeyDown(SDL_Keycode key){

    switch( key )
    {
        case SDLK_SPACE:
            paused = !paused;
        break;

        case SDLK_ESCAPE:
            running = false;
        break;

        case SDLK_UP:
        break;

        case SDLK_DOWN:
        break;

        case SDLK_LEFT:
        break;

        case SDLK_RIGHT:
        break;

        case SDLK_l:
        break;

        case SDLK_k:
        break;

        case SDLK_j:
        break;

        case SDLK_h:
        break;

        case SDLK_g:
        break;

        case SDLK_f:
        break;

        case SDLK_d:
        break;

        default:
        break;
    }
}

void CGame::OnKeyUp(SDL_Keycode key){
}

void CGame::LoadMainMenu(){

}

bool CGame::OnLButtonDown(int x, int y){


    if(MenuManager.HandleLClick(x,y)){
        return true;
    }

    // Player->MoveTo(mouseTileX, mouseTileY);

    return true;
}

bool CGame::OnRButtonDown(int x, int y){
    if(MenuManager.HandleRClick(x,y)){
        return true;
    }
    return false;
}

void CGame::UpdatePlayerPosition(){
    if(!Player->GetBusyTime() || Player->IsIdle()){
        int tx = Player->getTileX();
        int ty = Player->getTileY();
        const Uint8 *keystate = SDL_GetKeyboardState(NULL);
        if (keystate[SDL_SCANCODE_LEFT] ) {
            tx--;
        }
        if (keystate[SDL_SCANCODE_A] ) {
            tx--;
        }
        if (keystate[SDL_SCANCODE_RIGHT] ) {
            tx++;
        }
        if (keystate[SDL_SCANCODE_D] ) {
            tx++;
        }
        if (keystate[SDL_SCANCODE_UP] ) {
            ty--;
        }
        if (keystate[SDL_SCANCODE_W] ) {
            ty--;
        }
        if (keystate[SDL_SCANCODE_DOWN] ) {
            ty++;
        }
        if (keystate[SDL_SCANCODE_S] ) {
            ty++;
        }
        if( tx != Player->getTileX() || ty != Player->getTileY()){
            Player->ClearActions();
            Player->AddAction(CAction(CAction::moveTile, tx, ty));
        }
    }

    SetViewPort(
        Player->getX() - (CScreen::screen_half_w / ZoomLvl()) + (Player->getW() / 2),
        Player->getY() - (CScreen::screen_half_h / ZoomLvl()) +  (Player->getH() / 2));
}


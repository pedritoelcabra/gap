/// GAP - Main file

#include "CGame.h"

CGame GAP;


CGame::CGame(){
    running = true;
    paused = false;
    seed =  time(0);
}

int CGame::OnExecute(){

    tick = 0;
    Uint32 thisFrame = SDL_GetTicks();
    Uint32 frameTime = 0;
    frameCount = 0;
    framesUntilLastSecond = 0;
    currentSecond = 0;
    fps = 0;


    if (OnInit() == false) {
        return -1;
    }

    SDL_Event event;
    while (running){

        thisFrame = SDL_GetTicks();
        while (SDL_PollEvent(&event)){
            OnEvent(&event);
        }
        if(!paused){
            OnProcessTick();
        }
        OnRenderGL();
        //OnRenderGL();

        // frame control
        frameTime = SDL_GetTicks() - thisFrame;
        if( GAP.frameMS > frameTime){
            SDL_Delay(GAP.frameMS - frameTime);
        }
        frameCount++;
        if(int(thisFrame/1000) > currentSecond){
            currentSecond = int(thisFrame/1000);
            fps = frameCount - framesUntilLastSecond;
            framesUntilLastSecond = frameCount;
        }
    }
    OnCleanUp();

    return 0;
}

int main(int argc, char* argv[]){

    return GAP.OnExecute();

    exit(0);
}

/// PPP - Screen constants

#ifndef _SCREEN_H
#define _SCREEN_H

#include <SDL2/SDL.h>
#include <utility>

class CScreen
{
public:

    CScreen() {;}

    ~CScreen() {;}

    static const unsigned int       screen_bpp = 32;       //colour depth
    static const unsigned int       screen_h = 768;        //height in pixel
    static const unsigned int       screen_half_h = screen_h / 2;        //height in pixel
    static const unsigned int       screen_w = 1366;       //width in pixel
    static const unsigned int       screen_half_w = screen_w / 2;       //width in pixel
    static const int                tileWidth = 32;
    static const int                tileWidthH = tileWidth / 2;
    static const int                unitWidth = 64;
    static const int                tilesPerChunk = 32;
    static constexpr float          diagonalRatio = 1.414;
    static constexpr float          impassableCost = 3.0f;
    static constexpr float          flatMoveCost = 2.0f;
    static constexpr float          pathMoveCost = 1.0f;
    static constexpr float          roadMoveCost = 0.0f;
    static constexpr float          waterLevel = -0.5f;
    static constexpr float          beachLevel = -0.4f;
    static constexpr float          grassLevel = 0.1f;
    static constexpr float          dryLandLevel = 0.6f;
    static const int                buildingUpdateFrequency = 60;
    static const int                itemIconSize = 24;


    static SDL_Color                colorBlack(){return {255, 255, 255, 255};};
    static SDL_Color                colorGreen(){return {100, 100, 255, 255};};

};

typedef std::pair<int, int>    vec2i;

#endif // _SCREEN_H

// move cost big road 0+
// move cost small road 1+
// move cost land 2+
// move cost building 3+
// move cost sea 4+
// move cost mountain 5+
// move cost impassable 10

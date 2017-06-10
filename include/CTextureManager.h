#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <map>
#include <vector>
#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#define GLEW_STATIC
#include "gl/glew.h"
#include <SDL2/SDL_opengl.h>
#include "SDL2/SDL_gpu.h"

typedef std::map<std::string, GPU_Image*>     TextureMapGL;
typedef std::map<int, TTF_Font*>                FontMap;
class CTextureManager
{
    public:
        CTextureManager(){};
        virtual ~CTextureManager(){};

        int                 LoadTextureGL(std::string name, std::string path);
        void                DrawTextureGL(std::string* name, GPU_Rect* src_rect, GPU_Rect* dest_rect, bool absolutePos = false);
        void                DrawTextureGL(GPU_Image* texture, GPU_Rect* src_rect, GPU_Rect* dest_rect, bool absolutePos = false);
        void                DrawHighLightCircle(int tileX, int tileY, int radius, SDL_Color  color);
        void                DrawConnectionLine(int t1X, int t1Y, int t2X, int t2Y, SDL_Color color);
        void                DrawRectangle(int x1, int y1, int x2, int y2, SDL_Color color);
        bool                LoadFont(std::string path);
        void                Init();
        GPU_Image*          GetTexture(std::string* name);
        TTF_Font*           GetFont(int fontSize);
        GPU_Rect*           GetClip(int frame);
        GPU_Rect*           GetIconClip32(int frame);
        GLuint              programID;
    protected:
    private:
        const static int    spriteCount = 273;
        const static int    iconSheet32W = 64;
        const static int    iconSheet32H = 50;
        const static int    iconCount32 = iconSheet32W * iconSheet32H;
        GPU_Rect            SpriteClips[spriteCount];
        GPU_Rect            IconClips32[iconCount32];
        GPU_Rect            DrawRect;
        TextureMapGL        TexturesGL;
        FontMap             Fonts;
};

#endif // TEXTUREMANAGER_H

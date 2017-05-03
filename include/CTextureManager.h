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
#include "CSurface.h"

typedef std::map<std::string, SDL_Texture*>     TextureMap;
typedef std::map<std::string, GPU_Image*>     TextureMapGL;
typedef std::map<int, TTF_Font*>                FontMap;

class CTextureManager
{
    public:
        CTextureManager();
        virtual ~CTextureManager();

        int                 LoadTextureGL(std::string name, std::string path);
        void                DrawTextureGL(std::string name, GPU_Rect* src_rect, GPU_Rect* dest_rect, bool absolutePos = false);
        void                DrawTextureGL(GPU_Image* texture, GPU_Rect* src_rect, GPU_Rect* dest_rect, bool absolutePos = false);
        void                DrawHighLightCircle(int tileX, int tileY, int radius, SDL_Color  color);
        void                DrawConnectionLine(int t1X, int t1Y, int t2X, int t2Y, SDL_Color color);
        bool                LoadFont(std::string path);
        void                Init();
        GPU_Image*          GetTexture(std::string name);
        void                SetTexture(SDL_Texture* tex, std::string name);
        TTF_Font*           GetFont(int fontSize);
        GPU_Rect*           GetClip(int frame);
        GLuint              programID;
    protected:
    private:
        const static int    spriteCount = 273;
        GPU_Rect            SpriteClips[spriteCount];
        GPU_Rect            DrawRect;
        TextureMap          Textures;
        TextureMapGL        TexturesGL;
        FontMap             Fonts;
        GLuint              texture[500];
        int                 textureCount;
};

#endif // TEXTUREMANAGER_H

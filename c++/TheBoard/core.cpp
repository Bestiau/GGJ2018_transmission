#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

#include "core.h"

void ligne(int x1, int y1, int x2, int y2, Uint32 r,  Uint32 g,  Uint32 b);
SDL_Surface *cursor = NULL;
SDL_Surface *pinhole = NULL;
SDL_Surface *SplashImage = NULL;

SDL_Surface* LoadImage(char* fileName)
{
    SDL_Surface* imageLoaded = NULL;
    SDL_Surface* processedImage = NULL;

    imageLoaded = IMG_Load(fileName);

    if(imageLoaded != NULL)
    {
        processedImage = SDL_DisplayFormatAlpha(imageLoaded);
        SDL_FreeSurface(imageLoaded);

        if( processedImage != NULL )
        {
            Uint32 colorKey = SDL_MapRGB( processedImage->format, 0xFF, 0, 0xFF );
            SDL_SetColorKey( processedImage, SDL_SRCCOLORKEY, colorKey );
        }

    }
    else
    {
        printf("%s", IMG_GetError());
    }

    return processedImage;
}
void DrawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y)
{
    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;

    SDL_BlitSurface( image, NULL, destSurface, &destRect);
}
void DrawFrame(SDL_Surface *surface, SDL_Surface *dst, int x, int y, int w, int h, int frame)
{
    SDL_Rect destPos;
    destPos.x = x;
    destPos.y = y;

    int column = surface->w/w;
    SDL_Rect cutPos;
    cutPos.x = (frame%column)*w;
    cutPos.y = (frame/column)*h;
    cutPos.w = w;
    cutPos.h = h;
    SDL_BlitSurface(surface, &cutPos, dst, &destPos);
}

void drawMap()
{
    for(int y = 0; y < 10;++y )
    {
        for(int x = 0; x < 10; x++)
        {
            DrawImage(pinhole, Backbuffer, x * 32, y * 32);
        }
    }
}

void FillRect(SDL_Surface *surface, int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b)
{
    Uint32 color;

    color = SDL_MapRGB(surface->format, r, g, b);

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    SDL_FillRect(surface, &rect, color);
}


void DrawPixel(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
    if(SDL_MUSTLOCK(surface))
    {
        if(SDL_LockSurface(surface)<0)
            return;
    }
    if(x >= surface->w || x < 0 || y >= surface->h || y < 0)
    {
        return;
    }
    Uint32 *buffer;
    Uint32 color;

    color = SDL_MapRGB(surface->format, r, g, b);
    buffer = (Uint32*)surface->pixels + y*surface->pitch/4 + x;
    (*buffer)=color;
    if(SDL_MUSTLOCK(surface))
        SDL_UnlockSurface(surface);
}

void DrawRect(SDL_Surface* surface, int x, int y,
int width, int height,
Uint8 r, Uint8 g, Uint8 b)
{
    FillRect(surface, x, y, width, 1, r, g, b);
    FillRect(surface, x, y+height-1, width, 1, r, g, b);
    FillRect(surface, x, y, 1, height, r, g, b);
    FillRect(surface, x+width-1, y, 1, height, r, g, b);
}

void UpdateSplash()
{
    Uint8 *keys = SDL_GetKeyState(NULL);

    if(keys[SDLK_RETURN])
    {
        //ResetGame();
        gameState = GS_RUNNING;
    }
}


void DrawText(SDL_Surface *font, SDL_Surface *dst, int x, int y, char str[], int charSize)
{
    int ttw = strlen(str)*charSize;
    FillRect(dst, x-(charSize/2),y-(charSize/2),ttw+charSize,charSize*2, 25, 25, 25);
    DrawRect(dst, x-(charSize/2),y-(charSize/2),ttw+charSize,charSize*2,255, 255, 255);
    int xx;
    for(unsigned int i= 0; i < strlen(str); i++)
    {
        xx=charSize*i+x;
        if(str[i]=='\n')
        {
            y+=charSize+4;
            xx=charSize+x;
            continue;
        }

        DrawFrame(font, dst, xx, y, charSize, charSize, str[i]-32);
    }
}



void DrawImageFrame(SDL_Surface* image, SDL_Surface* destSurface, int x, int y, int width, int height, int frame)
{
    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;

    int columns = image->w/width;

    SDL_Rect sourceRect;
    sourceRect.y = (frame/columns)*height;
    sourceRect.x = (frame%columns)*width;
    sourceRect.w = width;
    sourceRect.h = height;

    SDL_BlitSurface( image, &sourceRect, destSurface, &destRect);
}

void DrawText(SDL_Surface* surface, char* string, int x, int y, TTF_Font* font, Uint8 r, Uint8 g, Uint8 b)
{
    SDL_Surface* renderedText = NULL;

    SDL_Color color;

    color.r = r;
    color.g = g;
    color.b = b;

    renderedText = TTF_RenderText_Solid( font, string, color );

    SDL_Rect pos;

    pos.x = x;
    pos.y = y;

    SDL_BlitSurface( renderedText, NULL, surface, &pos );
    SDL_FreeSurface(renderedText);
}

bool ProgramIsRunning()
{
    SDL_Event event;

    bool running = true;

    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
            running = false;

        if( event.type == SDL_KEYDOWN )
        {
            if(event.key.keysym.sym == SDLK_ESCAPE)
            {
                 if(gameState == GS_RUNNING)
                    gameState = GS_PAUSED;
                else if(gameState == GS_PAUSED)
                    gameState = GS_RUNNING;
            }
        }
    }

    return running;
}


bool InitSDL()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
        return false;

    if(Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 2048 ) == -1)
    {
        return false;
    }

    if(TTF_Init() == -1)
    {
        return false;
    }

    Backbuffer = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE );

    if(Backbuffer == NULL)
        return false;

    return true;
}

void FreeFiles()
{
    if(cursor !=NULL)
        SDL_FreeSurface(cursor);
    cursor = NULL;

}


void FreeGame()
{
	FreeFiles();
    Mix_HaltMusic();
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
}


//*****************************************************************************
//File loading

bool LoadFiles()
{
    cursor = LoadImage("assets/cursor.png");
    pinhole = LoadImage("assets/hole.png");
    SplashImage = LoadImage("assets/splash.png");
}


bool InitGame()
{
    if(!InitSDL())
        return false;


    SDL_ShowCursor(SDL_DISABLE);
    LoadFiles();

    SDL_WM_SetCaption("SwitcherBoard",NULL);

   // Mix_PlayMusic(GameMusic, -1);

    gameState = GS_SPLASH;

    return true;
}

//*****************************************************************************

void DrawSplash()
{

    FillRect(Backbuffer, 0, 0, 640, 480, 150, 150, 255);
    DrawImage(SplashImage, Backbuffer, 0, 0);
    int x, y;
    ligne(0,0,50,136,255,255,0);
    SDL_GetMouseState(&x, &y);
    DrawImage(cursor, Backbuffer, x, y);
}


void RunGame()
{
    switch(gameState)
    {
    case GS_SPLASH:
        UpdateSplash();
        break;
    case GS_RUNNING:
        //UpdateGame();
        break;
    case GS_GAMEOVER:
        //UpdateGameOver();
        break;
    default:
        break;
    }
}


void echangerEntiers(int* x, int* y)
{
  int t = *x;
  *x = *y;
  *y = t;
}

void ligne(int x1, int y1, int x2, int y2, Uint32 r,  Uint32 g,  Uint32 b)
{
  int d, dx, dy, aincr, bincr, xincr, yincr, x, y;

  if (abs(x2 - x1) < abs(y2 - y1)) {
    /* parcours par l'axe vertical */

    if (y1 > y2) {
      echangerEntiers(&x1, &x2);
      echangerEntiers(&y1, &y2);
    }

    xincr = x2 > x1 ? 1 : -1;
    dy = y2 - y1;
    dx = abs(x2 - x1);
    d = 2 * dx - dy;
    aincr = 2 * (dx - dy);
    bincr = 2 * dx;
    x = x1;
    y = y1;

    DrawPixel(Backbuffer, x, y , r, g, b);

    for (y = y1+1; y <= y2; ++y) {
      if (d >= 0) {
	x += xincr;
	d += aincr;
      } else
	d += bincr;

          DrawPixel(Backbuffer, x, y , r, g, b);
    }

  } else {
    /* parcours par l'axe horizontal */

    if (x1 > x2) {
      echangerEntiers(&x1, &x2);
      echangerEntiers(&y1, &y2);
    }

    yincr = y2 > y1 ? 1 : -1;
    dx = x2 - x1;
    dy = abs(y2 - y1);
    d = 2 * dy - dx;
    aincr = 2 * (dy - dx);
    bincr = 2 * dy;
    x = x1;
    y = y1;

        DrawPixel(Backbuffer, x, y , r, g, b);

    for (x = x1+1; x <= x2; ++x) {
      if (d >= 0) {
	y += yincr;
	d += aincr;
      } else
	d += bincr;

          DrawPixel(Backbuffer, x, y , r, g, b);
    }
  }
}

void DrawScreen()
{
    switch(gameState)
    {
    case GS_SPLASH:
        DrawSplash();
        break;
    case GS_RUNNING:
        //DrawGame();
        drawMap();
            int x, y;
    SDL_GetMouseState(&x, &y);
    DrawImage(cursor, Backbuffer, x, y);
        break;
    case GS_GAMEOVER:
        //DrawGameOver();
        break;
    case GS_PAUSED:
        //DrawGamePaused();
        break;
    default:
        break;
    }
}

//****************************************************************************


#ifndef CORE_H_INCLUDED
#define CORE_H_INCLUDED

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
const int GS_SPLASH = 0;
const int GS_RUNNING = 1;
const int GS_GAMEOVER = 2;
const int GS_PAUSED = 3;
int gameState;

SDL_Surface *Backbuffer;


SDL_Surface* LoadImage(char* fileName);
void DrawFrame(SDL_Surface *surface, SDL_Surface *dst, int x, int y, int w, int h, int frame);
void DrawText(SDL_Surface *font, SDL_Surface *dst, int x, int y, char str[], int charSize);
void DrawText(SDL_Surface* surface, char* string, int x, int y, TTF_Font* font, Uint8 r, Uint8 g, Uint8 b);
bool ProgramIsRunning();
bool InitGame();
void FreeGame();
void DrawPixel(SDL_Surface *surface, int x, int y, Uint8 r, Uint8 g, Uint8 b);
void FillRect(SDL_Surface *surface, int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b);

#endif // CORE_H_INCLUDED

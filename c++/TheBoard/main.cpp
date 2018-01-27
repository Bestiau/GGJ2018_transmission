#include <iostream>
#include <SDL/SDL.h>

extern bool InitGame();
extern bool ProgramIsRunning();
extern void RunGame();
extern SDL_Surface *Backbuffer;
extern void DrawScreen();
extern void   FreeGame();

const int FPS = 30;
const int FRAME_DELAY = 1000/FPS;

int main(int argc, char *argv[])
{
    if(!InitGame())
    {
        return 0;
    }

    while(ProgramIsRunning())
    {
        long int oldTime = SDL_GetTicks();
        SDL_FillRect(Backbuffer, NULL, 0);
        RunGame();
        DrawScreen();


        int frameTime = SDL_GetTicks() - oldTime;

        if(frameTime < FRAME_DELAY)
            SDL_Delay(FRAME_DELAY - frameTime);
        SDL_Flip(Backbuffer);
    }

    FreeGame();

    return 0;
}

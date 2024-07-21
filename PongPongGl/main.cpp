#include "Application.h"
#include "Constants.h"

int main(int argc, char *argv[])
{
    //CONST
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;
    //CONST

    int frameStart;
    int frameTime;

    App* app = new App();

    app->init("FAPP", SCREEN_WIDTH, SCREEN_HEIGHT, false);

    while(app->isAppRunning())
    {
        frameStart = SDL_GetTicks();

        app->handleEvents();
        app->update();
        app->render();

        frameTime = SDL_GetTicks() - frameStart;
        if(frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    app->clean();
    return 0;
}

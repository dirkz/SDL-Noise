#include "stdafx.h"

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>

#include "AppState.h"

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    try
    {
        AppState *appState = new AppState();
        *appstate = appState;
    }
    catch (...)
    {
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *appState = reinterpret_cast<AppState *>(appstate);

    try
    {
        appState->Iterate();
    }
    catch (...)
    {
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    AppState *appState = reinterpret_cast<AppState *>(appstate);
    delete appState;
}

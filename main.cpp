#include <iostream>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void sdlErr(std::string);

int main(int argc, char * argv[]) {
    // a window to render things to
    SDL_Window * window = NULL;

    // a surface to be contained by the window
    SDL_Surface * windowSurface = NULL;

    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        sdlErr("Failed to initialize SDL.");
    } else {
        window = SDL_CreateWindow( "SDL Learning", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
            sdlErr("Window could not be created.");
        else {
            // get the window surface
            windowSurface = SDL_GetWindowSurface(window);
            // make that surface white
            SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface->format, 0xFF, 0xFF, 0xFF));
            // update the surface
            SDL_UpdateWindowSurface(window);
            // wait 5 seconds
            SDL_Delay(5000);
        }
    }

	//Destroy window
	SDL_DestroyWindow( window );

	//Quit SDL subsystems
	SDL_Quit();
    return(0);
}

void sdlErr(std::string message){
    std::cerr << message << '\n'
              << "SDL Error: " << SDL_GetError() << '\n';
}

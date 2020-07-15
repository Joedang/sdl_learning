#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void sdlErr(std::string);
void imgErr(std::string);

int main(int argc, char * argv[]) {
    // a window to render things to
    SDL_Window * window = NULL;

    // a surface to be contained by the window
    SDL_Surface * windowSurface = NULL;

    // test image
    SDL_Surface * doge = NULL;

    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        sdlErr("Failed to initialize SDL.");
    } else {
        window = SDL_CreateWindow( "SDL Learning", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL)
            sdlErr("Window could not be created.");
        else {
            if (IMG_Init(IMG_INIT_JPG) < 0)
                sdlErr("Failed to initialize image support.");
            // get the window surface
            windowSurface = SDL_GetWindowSurface(window);
            // get the image to display
            doge = IMG_Load("img/doge.jpg");
            if (doge == NULL)
                imgErr("Failed to load doge.");
            //doge = SDL_LoadBMP("img/doge.bmp");
            //if (doge == NULL)
            //    sdlErr("Failed to load doge.");
            // make that surface white
            SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface->format, 0xFF, 0xFF, 0xFF));
            // draw the doge to the screen
            SDL_BlitSurface(doge, NULL, windowSurface, NULL);
            // update the surface
            SDL_UpdateWindowSurface(window);
            // wait 3 seconds
            SDL_Delay(3000);
        }
    }

	// free resources
	SDL_DestroyWindow( window );
    SDL_FreeSurface(doge);

	//Quit SDL subsystems
    IMG_Quit();
	SDL_Quit();
    return(0);
}

void sdlErr(std::string message){
    std::cerr << message << '\n'
              << "SDL Error: " << SDL_GetError() << '\n';
}
void imgErr(std::string message){
    std::cerr << message << '\n'
              << "SDL_Image Error: " << IMG_GetError() << '\n';
}

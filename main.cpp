#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void sdlErr(std::string);// report an error related to SDL
void imgErr(std::string);// report an error related to SDL_image

int main(int argc, char * argv[]) {

    SDL_Window * window = NULL;// a window to render things to
    SDL_Surface * windowSurface = NULL;// a surface to be contained by the window
    SDL_Surface * chris = NULL;// test image

    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        sdlErr("Failed to initialize SDL.");

    // create a window
    window = SDL_CreateWindow( "SDL Learning", 
                                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                SCREEN_WIDTH, SCREEN_HEIGHT, 
                                SDL_WINDOW_SHOWN
                             );
    if (window == NULL)
        sdlErr("Window could not be created.");

    if (IMG_Init(IMG_INIT_JPG) < 0)// load the libraries for loading images
        sdlErr("Failed to initialize image support.");

    windowSurface = SDL_GetWindowSurface(window);// get the reference to the window's surface (framebuffer)

    chris = IMG_Load("img/chris.jpg");
    if (chris == NULL)
        imgErr("Failed to load chris.");
    //doge = SDL_LoadBMP("img/doge.bmp");
    //if (doge == NULL)
    //    sdlErr("Failed to load doge.");

    // draw stuff
    SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface->format, 0xFF, 0xFF, 0xFF));// make that surface white
    SDL_BlitSurface(chris, NULL, windowSurface, NULL);// draw the chris to the screen
    SDL_UpdateWindowSurface(window);// update the surface

    SDL_Delay(3000);// wait 3 seconds

	// free resources
	SDL_DestroyWindow(window);// This also destroys the framebuffer referred to by windowSurface.
    SDL_FreeSurface(chris);

	//Quit SDL subsystems
    IMG_Quit();
	SDL_Quit();
    return 0;
}

void sdlErr(std::string message){
    std::cerr << message << '\n'
              << "SDL Error: " << SDL_GetError() << '\n';
}
void imgErr(std::string message){
    std::cerr << message << '\n'
              << "SDL_Image Error: " << IMG_GetError() << '\n';
}

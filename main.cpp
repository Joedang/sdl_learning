#include <iostream> // handle input/output streams (useful for piping text)
#include <stdio.h> // nice functions for formatting/printing strings
#include <SDL2/SDL.h> // API for handling multi-media user interaction
#include <SDL2/SDL_image.h> // extension to SDL for loading non-BMP images

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void sdlErr(std::string);// report an error related to SDL
void imgErr(std::string);// report an error related to SDL_image

int main(int argc, char * argv[]) {

    SDL_Window * window = NULL;// a window to render things to
    SDL_Surface * windowSurface = NULL;// a surface to be contained by the window
    SDL_Surface * chris = NULL;// test image
    char chrisFile[] = "img/chris.bmp";// test file to use
    SDL_Rect chrisLocation;// where to draw the test image
        chrisLocation.x = 50;
        chrisLocation.y = 50;
    SDL_Rect dstrect_dummy = chrisLocation;// for non-destructive location drawing
    int chrisSpeed = 10;// how much to move chris per itteration
    SDL_Event event;// event handler
    bool quit = false;// whether to quit the program

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

    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0)// load the libraries for loading images
        sdlErr("Failed to initialize image support.");

    windowSurface = SDL_GetWindowSurface(window);// get the reference to the window's surface (framebuffer)

    chris = IMG_Load(chrisFile);
    if (chris == NULL)
        imgErr("Failed to load chris.");
    //doge = SDL_LoadBMP(chrisFile);
    //if (doge == NULL)
    //    sdlErr("Failed to load chris.");


    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type){
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym){
                        case SDLK_UP:
                            chrisLocation.y -= chrisSpeed;
                            break;
                        case SDLK_DOWN:
                            chrisLocation.y += chrisSpeed;
                            break;
                        case SDLK_LEFT:
                            chrisLocation.x -= chrisSpeed;
                            break;
                        case SDLK_RIGHT:
                            chrisLocation.x += chrisSpeed;
                            break;
                    }
                    break;
                default:
                    //std::cout << "SDL event type: " << std::to_string(event.type) << std::endl;
                    break;
            }
        }
        // TODO: I would like to know why keypresses seem to send events as if the program were a text editor, 
        //       rather than just recognizing down/up events.
        //       Pressing and holding an arrow key, the image moves one step, waits, and then moves smoothly.
        // draw stuff
        SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface->format, 0xAA, 0xAA, 0xAA));// fill that surface with a color
        dstrect_dummy = chrisLocation;
        SDL_BlitSurface(chris, NULL, windowSurface, &dstrect_dummy);// draw the chris to the screen
        SDL_UpdateWindowSurface(window);// update the surface

        SDL_Delay(100);// wait some ms
    }

	// free resources
	SDL_DestroyWindow(window);// This also destroys the framebuffer referred to by windowSurface.
    SDL_FreeSurface(chris);

	//Quit SDL subsystems
    std::cout << "Goodbye." << std::endl;
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

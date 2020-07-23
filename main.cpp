// includes {{{
#include <iostream> // handle input/output streams (useful for piping text)
#include <stdio.h> // nice functions for formatting/printing strings
#include <SDL2/SDL.h> // API for handling multi-media user interaction
#include <SDL2/SDL_image.h> // extension to SDL for loading non-BMP images
// }}}
// globals {{{
const int SCREEN_WIDTH = 1080; //640;
const int SCREEN_HEIGHT = 720; //480;
// }}}
// prototypes {{{
void sdlErr(std::string);// report an error related to SDL
void imgErr(std::string);// report an error related to SDL_image
void printPixelFmt(SDL_PixelFormat);// verbosely print all the data in a pixel format
// }}}
int main(int argc, char * argv[]) { // {{{
    // variable declarations {{{
    std::cout << "declaring variables" << std::endl;
    SDL_Window * window = NULL;// a window to render things to
    SDL_Renderer * renderer = NULL;// window renderer
    //SDL_Surface * windowSurface = NULL;// a surface to be contained by the window
    SDL_Surface * chris = NULL;// test image
    SDL_Texture * chrisTex = NULL;
    SDL_Surface * surfaceHelper = NULL;
    SDL_Surface * screenBG = NULL;// screen background
    SDL_Rect screenBG_rect;// a rectangle that fills the screen
        screenBG_rect.x = 0;
        screenBG_rect.y = 0;
        screenBG_rect.w = SCREEN_WIDTH;
        screenBG_rect.h = SCREEN_HEIGHT;
    Uint8 bgAlpha = 0xFF;// opacity of the background
    int breatheDir = -1;// increment by which bgAlpha is changing
    //SDL_Texture * bgTex = NULL;
    char chrisFile[] = "img/chris.png";// test file to use
    //char chrisFile[] = "img/chris24.bmp";// test file to use
    SDL_Rect chrisLocation;// where to draw the test image
        chrisLocation.x = 0;
        chrisLocation.y = 0;
        chrisLocation.w = SCREEN_WIDTH;// if we want him to fill the screen
        chrisLocation.h = SCREEN_HEIGHT;
    float chrisScale = 4;// factor by which to increase the size of the chris "player"
    char boiFile[] = "img/datBoi_reel.png";
    SDL_Texture * boiTex = NULL;
    int boiFrames = 5;
    SDL_Rect boiSrc = {0,0,0,0};// the current "frame" on the animation "reel"
    SDL_Rect boiDest = {300,100,100,100};// where the animated sprite will be drawn
    SDL_Rect drawRect = {0,0,0,0};// used for drawing simple geometry
    int chrisSpeed = 10;// how much to move chris per itteration
    int vx = 0;// x velocity
    int vy = 0;// y velocity
    bool leftHeld = false;// whether the left key is being held
    bool rightHeld = false;
    bool upHeld = false;
    bool downHeld = false;
    SDL_Event event;// event handler
    SDL_ScaleMode scaleMode = SDL_ScaleModeLinear; // nearest, linear, or best (anisotropic)
    int delayTime = 50;// ms, delay between frames
    bool quit = false;// whether to quit the program

    // dummy variables
    int i = 0;// used for counting
    int w = 0;// width
    int h = 0;// height
    int access = 0;// the access type of a texture
    Uint32 format = 0;// pixel format
    SDL_Rect dstrect_dummy = chrisLocation;// for non-destructive location drawing
    // }}}

    // initialization (similar to lazyfoo's init()) {{{
    std::cout << "initializing..." << std::endl;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        sdlErr("Failed to initialize SDL.");
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) // nearest-pixel (0) may be preferrable for pixel art
        sdlErr("Linear texture filtering is not enabled.");
    window = SDL_CreateWindow( "SDL Learning", 
                                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                                SCREEN_WIDTH, SCREEN_HEIGHT, 
                                SDL_WINDOW_SHOWN
                             );
    if (window == NULL)
        sdlErr("Failed to create window.");
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
        sdlErr("The renderer could not be created.");
    if (SDL_SetRenderDrawColor(renderer, 0xFF, 0xAA, 0xAA, 0xFF))// color for Rect, Line, and Clear; RGBA
        sdlErr("Failed to set renderer draw color.");
    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0)// load the libraries for loading images
        sdlErr("Failed to initialize image support.");
    //windowSurface = SDL_GetWindowSurface(window);// get the reference to the window's surface (framebuffer)
    // }}}
    std::cout << "Loading texture directly from file..." << std::endl;
    chrisTex = IMG_LoadTexture(renderer, chrisFile);
    if (chrisTex == NULL)
        imgErr("Failed to load chris texture from file.");
    // Get the width and height from chrisTex and put them into the chrisLocation width and height.
    // This ensures that "player" is the size of the image.
    SDL_QueryTexture(chrisTex, &format, &access, &chrisLocation.w, &chrisLocation.h);
    chrisLocation.w *= chrisScale;// make chris bigger
    chrisLocation.h *= chrisScale;
    boiTex = IMG_LoadTexture(renderer, boiFile);
    if (boiTex == NULL)
        imgErr("Failed to load boi texture from file.");
    SDL_QueryTexture(boiTex, &format, &access, &boiSrc.w, &boiSrc.h);
    boiSrc.w = boiSrc.w/boiFrames;
    std::cout << "boiSrc.w: " << boiSrc.w << std::endl;
    boiDest.w = boiSrc.w;
    boiDest.h = boiSrc.h;
    std::cout << "Setting texture scale mode: " << scaleMode << "..." << std::endl;
    SDL_SetTextureScaleMode(chrisTex, scaleMode);
    SDL_SetTextureScaleMode(boiTex, scaleMode);
    // }}}
    // main loop {{{
    std::cout << "entering main loop..." << std::endl;
    // user input stuff {{{
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type){
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    std::cout << "pressed: ";
                    switch (event.key.keysym.sym){
                        case SDLK_UP:
                            std::cout << "up" << std::endl;
                            upHeld = true;
                            break;
                        case SDLK_DOWN:
                            std::cout << "down" << std::endl;
                            downHeld = true;
                            break;
                        case SDLK_LEFT:
                            std::cout << "left" << std::endl;
                            leftHeld = true;
                            break;
                        case SDLK_RIGHT:
                            std::cout << "right" << std::endl;
                            rightHeld = true;
                            break;
                        default:
                            std::cout << "unknown" << std:: endl;
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    std::cout << "released: ";
                    switch (event.key.keysym.sym){
                        case SDLK_UP:
                            std::cout << "up" << std::endl;
                            upHeld = false;
                            break;
                        case SDLK_DOWN:
                            std::cout << "down" << std::endl;
                            downHeld = false;
                            break;
                        case SDLK_LEFT:
                            std::cout << "left" << std::endl;
                            leftHeld = false;
                            break;
                        case SDLK_RIGHT:
                            std::cout << "right" << std::endl;
                            rightHeld = false;
                            break;
                        default:
                            std::cout << "unknown" << std:: endl;
                            break;
                    }
                default:
                    //std::cout << "SDL event type: " << std::to_string(event.type) << std::endl;
                    break;
            }
        }
        // }}}

        // move chris
        chrisLocation.x = (chrisLocation.x + (rightHeld - leftHeld)*chrisSpeed )% SCREEN_WIDTH;
        chrisLocation.y = (chrisLocation.y + (downHeld - upHeld)*chrisSpeed )% SCREEN_HEIGHT;
        // wrap the chris pacman-style
        if (chrisLocation.x < 0)
            chrisLocation.x += SCREEN_WIDTH;
        if (chrisLocation.y < 0)
            chrisLocation.y += SCREEN_HEIGHT;
        chrisLocation.x %= SCREEN_WIDTH;
        chrisLocation.y %= SCREEN_HEIGHT;

        // draw stuff {{{ 
        SDL_SetRenderDrawColor(renderer, 0x33, 0x11, 0x11, 0xFF);// default color if nothing is drawn [to full opacity]
        //SDL_RenderClear(renderer);// clear the screen. You can comment this to get a cheap trail effect on the "player".
        // draw a gray filled rectangle
        SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, 0xFF);
        drawRect = SDL_Rect {50, 50, 50, 100};// left, top, width, height
        SDL_RenderFillRect(renderer, &drawRect);
        // draw a green outlined rectangle
        SDL_SetRenderDrawColor(renderer, 0x00, 0xCC, 0x00, 0xFF);
        drawRect = SDL_Rect {50, 50, 100, 100};
        SDL_RenderDrawRect(renderer, &drawRect);
        // draw a blue line
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xCC, 0xFF);
        SDL_RenderDrawLine(renderer, 50,50,100,100);// x1, y1, x2, y2 (remember, y=0 is the top and y+ is down)
        // draw some red diagonal dots
        SDL_SetRenderDrawColor(renderer, 0xCC, 0x00, 0x00, 0xFF);
        for (i=0; i < 100; i++) {
            SDL_RenderDrawPoint(renderer, 3*i, 4*i);
        }
        if (bgAlpha >= 0xFF)
            breatheDir = -1;
        else if (bgAlpha <= 0)
            breatheDir = 1;
        bgAlpha += breatheDir;// make the background fade in and out
        SDL_SetTextureAlphaMod(chrisTex, bgAlpha);
        SDL_RenderCopy(renderer, chrisTex, NULL, &screenBG_rect);// render background texture
        SDL_SetTextureAlphaMod(chrisTex, 0xFF);
        // draw dat boi
        dstrect_dummy = boiDest;
        SDL_RenderCopy(renderer, boiTex, &boiSrc, &dstrect_dummy);// render foreground texture (scales instead of crops)
        boiSrc.x = (boiSrc.x + boiSrc.w) % (boiSrc.w * boiFrames);
        // draw chris
        dstrect_dummy = chrisLocation;
        SDL_RenderCopy(renderer, chrisTex, NULL, &dstrect_dummy);// render foreground texture (scales instead of crops)
        SDL_RenderPresent(renderer);// draw the rendering to the screen
        // }}}

        // This *really* helps ensure sane CPU usage.
        SDL_Delay(delayTime);// wait some ms
    }
    // }}}

	// Free resources and Quit SDL subsystems (like lazyfoo's close()) {{{
    std::cout << "cleaning up..." << std::endl;
	SDL_DestroyWindow(window);// This also destroys the framebuffer referred to by windowSurface.
    window = NULL;
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_FreeSurface(chris);
    chris = NULL;
    SDL_FreeSurface(surfaceHelper);
    surfaceHelper = NULL;
    SDL_FreeSurface(screenBG);
    screenBG = NULL;
    SDL_DestroyTexture(chrisTex);
    chrisTex = NULL; // TODO: I'm not sure if it's necessary to set stuff to NULL like this.
    //SDL_DestroyTexture(bgTex);
    //bgTex = NULL;
    IMG_Quit();
	SDL_Quit();
    std::cout << "Goodbye." << std::endl;
    // }}}
    return 0;
}//}}}
// function definitions {{{
void sdlErr(std::string message){
    std::cerr << message << '\n'
              << "SDL Error: " << SDL_GetError() << '\n';
}
void imgErr(std::string message){
    std::cerr << message << '\n'
              << "SDL_Image Error: " << IMG_GetError() << '\n';
}

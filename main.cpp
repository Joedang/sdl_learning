#include <iostream> // handle input/output streams (useful for piping text)
#include <stdio.h> // nice functions for formatting/printing strings
#include <SDL2/SDL.h> // API for handling multi-media user interaction
#include <SDL2/SDL_image.h> // extension to SDL for loading non-BMP images

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void sdlErr(std::string);// report an error related to SDL
void imgErr(std::string);// report an error related to SDL_image
void printPixelFmt(SDL_PixelFormat);// verbosely print all the data in a pixel format

int main(int argc, char * argv[]) {
    std::cout << "declaring variables" << std::endl;
    SDL_Window * window = NULL;// a window to render things to
    SDL_Renderer * renderer = NULL;// window renderer
    SDL_Surface * windowSurface = NULL;// a surface to be contained by the window
    SDL_Surface * chris = NULL;// test image
    SDL_Texture * chrisTex = NULL;
    SDL_Surface * surfaceHelper = NULL;
    SDL_Surface * screenBG = NULL;// screen background
    SDL_Rect screenBG_rect;// a rectangle that fills the screen
        screenBG_rect.x = 0;
        screenBG_rect.y = 0;
        screenBG_rect.w = SCREEN_WIDTH;
        screenBG_rect.h = SCREEN_HEIGHT;
    //SDL_Texture * bgTex = NULL;
    char chrisFile[] = "img/chris.png";// test file to use
    //char chrisFile[] = "img/chris24.bmp";// test file to use
    SDL_Rect chrisLocation;// where to draw the test image
        chrisLocation.x = 0;
        chrisLocation.y = 0;
        chrisLocation.w = SCREEN_WIDTH;// if we want him to fill the screen
        chrisLocation.h = SCREEN_HEIGHT;
    SDL_Rect dstrect_dummy = chrisLocation;// for non-destructive location drawing
    int chrisSpeed = 10;// how much to move chris per itteration
    SDL_Event event;// event handler
    SDL_ScaleMode scaleMode = SDL_ScaleModeLinear; // nearest, linear, or best (anisotropic)
    bool quit = false;// whether to quit the program

    // initialization (similar to lazyfoo's init())
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
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        sdlErr("The renderer could not be created.");
    if (SDL_SetRenderDrawColor(renderer, 0xFF, 0xAA, 0xAA, 0xFF))// color for Rect, Line, and Clear; RGBA
        sdlErr("Failed to set renderer draw color.");
    if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0)// load the libraries for loading images
        sdlErr("Failed to initialize image support.");
    windowSurface = SDL_GetWindowSurface(window);// get the reference to the window's surface (framebuffer)

    // similar to lazyfoo's loadTexture()
    std::cout << "loading image(s)..." << std::endl;
    chris = IMG_Load(chrisFile);
    //chris = SDL_LoadBMP(chrisFile);
    if (chris == NULL)
        imgErr("Failed to load chris.");
    chrisLocation.w = chris->w;
    chrisLocation.h = chris->h;

    // Make the formats match. This prevents the conversion from ocurring on every blit.
    std::cout << "converting pixel formats..." << std::endl;
    std::cout << "window pixel format:" << std::endl;
    std::cout << "\t" << SDL_GetPixelFormatName(windowSurface->format->format) << std::endl;//figure out why this segfaults!... It helps to not uncomment pointer assignments, lol.
    //printf("window pixel format: %s\n", SDL_GetPixelFormatName(windowSurface->format->format));
    std::cout << "chris pixel format (pre-conversion:)" << std::endl;
    std::cout << "\t" << SDL_GetPixelFormatName(chris->format->format) << std::endl;
    //printf("chris pixel format (pre-conversion): %s\n", SDL_GetPixelFormatName(chris->format->format));
    std::cout << "actually doing it..." << std::endl;
    surfaceHelper = SDL_ConvertSurface(chris, windowSurface->format, 0);
    std::cout << "freeing chris surface..." << std::endl;
    SDL_FreeSurface(chris);// free the old surface
    chris = surfaceHelper;// swap the pointers around
    surfaceHelper = NULL; 
    printf("chris pixel format (post-conversion): %s\n", SDL_GetPixelFormatName(chris->format->format));
    std::cout << "creating texture from surface..." << std::endl;
    chrisTex = SDL_CreateTextureFromSurface(renderer, chris);
    if (chrisTex == NULL)
        sdlErr("Failed to create texture from surface.");
    std::cout << "Setting texture scale mode: " << scaleMode << "..." << std::endl;
    SDL_SetTextureScaleMode(chrisTex, scaleMode);
    //std::cout << "Saving a copy of the chris surface..." << std::endl;
    //if (SDL_SaveBMP(chris, "chris_surface.bmp") < 0)
    //    sdlErr("Failed to save a copy of the chris surface.");

    // prepare a surface to serve as a background
    // Use a copy of the window surface to define the extents and format of the screen background.
    //std::cout << "preparing background..." << std::endl;
    //screenBG = SDL_ConvertSurface(windowSurface, windowSurface->format, 0);
    //dstrect_dummy = screenBG_rect;
    //SDL_BlitScaled(chris, NULL, screenBG, &dstrect_dummy);
    //printf("screenBG pixel format: %s\n", SDL_GetPixelFormatName(screenBG->format->format));
    //bgTex = SDL_CreateTextureFromSurface(renderer, screenBG);
    //if (bgTex == NULL)
    //    sdlErr("Failed to load background texture.");

    // main loop
    std::cout << "entering main loop..." << std::endl;
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
        //SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface->format, 0xAA, 0xAA, 0xAA));// fill that surface with a color
        SDL_RenderClear(renderer);// clear the screen
        //dstrect_dummy = screenBG_rect;
        //SDL_BlitSurface(screenBG, NULL, windowSurface, &dstrect_dummy);// draw the background
        //SDL_RenderCopy(renderer, bgTex, NULL, NULL);// render background texture
        SDL_RenderCopy(renderer, chrisTex, NULL, &screenBG_rect);// render background texture
        dstrect_dummy = chrisLocation;
        //SDL_BlitSurface(chris, NULL, windowSurface, &dstrect_dummy);// draw the chris to the screen
        //SDL_BlitScaled(chris, NULL, windowSurface, &dstrect_dummy);// about twice as CPU-hungry with a 48x62 PNG
        //TODO: figure out why it's displaying big chris
        //TODO: It's still using surfaces to render the background. Make it use textures for everything.
        SDL_RenderCopy(renderer, chrisTex, NULL, &dstrect_dummy);// render foreground texture (scales instead of crops)
        //SDL_UpdateWindowSurface(window);// update the surface
        SDL_RenderPresent(renderer);// draw the rendering to the screen

        // This *really* helps ensure sane CPU usage.
        SDL_Delay(100);// wait some ms
    }


	// Free resources and Quit SDL subsystems (like lazyfoo's close())
    std::cout << "cleaning up..." << std::endl;
    SDL_DestroyTexture(chrisTex);
    chrisTex = NULL; // TODO: I'm not sure if it's necessary to set stuff to NULL like this.
    //SDL_DestroyTexture(bgTex);
    //bgTex = NULL;
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
	SDL_DestroyWindow(window);// This also destroys the framebuffer referred to by windowSurface.
    window = NULL;
    SDL_FreeSurface(chris);
    chris = NULL;
    IMG_Quit();
	SDL_Quit();
    std::cout << "Goodbye." << std::endl;
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
void printPixelFmt(SDL_PixelFormat fmt){
    printf("format name: %s\n", SDL_GetPixelFormatName(fmt.format));
    /*
    printf("palette:\n");
    printf("\tncolors: %0d\n", fmt.palette->ncolors);
    printf("\tcolors:\n");
    printf("\t\tr: %0d\n", fmt.palette->colors->r);
    printf("\t\tg: %0d\n", fmt.palette->colors->g);
    printf("\t\tb: %0d\n", fmt.palette->colors->b);
    printf("\t\tunused: %0d\n", fmt.palette->colors);
    printf("BitsPerPixel: %0d\n", fmt.BitsPerPixel);
    printf("BytesPerPixel: %0d\n", fmt.BytesPerPixel);
    printf("Rloss: %0d\n", fmt.Rloss);
    printf("Gloss: %0d\n", fmt.Gloss);
    printf("Bloss: %0d\n", fmt.Bloss);
    printf("Aloss: %0d\n", fmt.Aloss);
    printf("Rshift: %0d\n", fmt.Rshift);
    printf("Gshift: %0d\n", fmt.Gshift);
    printf("Bshift: %0d\n", fmt.Bshift);
    printf("Ashift: %0d\n", fmt.Ashift);
    printf("Rmask: %0d\n", fmt.Rmask);
    printf("Gmask: %0d\n", fmt.Gmask);
    printf("Bmask: %0d\n", fmt.Bmask);
    printf("Amask: %0d\n", fmt.Amask);
    */
    //printf("colorkey: %0d\n", fmt.colorkey);
    //printf("alpha: %0d\n", fmt.alpha);
    printf("Uint32 format: %0d\n", fmt.format);
    printf("SDL_Palette *palette: %0d\n", fmt.palette);
    if (fmt.palette != NULL){
        printf("palette:\n\n");
        printf("\tncolors: %0d\n\n", fmt.palette->ncolors);
        printf("\tcolors:\n\n");
        printf("\t\tr: %0d\n\n", fmt.palette->colors->r);
        printf("\t\tg: %0d\n\n", fmt.palette->colors->g);
        printf("\t\tb: %0d\n\n", fmt.palette->colors->b);
        printf("\t\tunused: %0d\n\n", fmt.palette->colors);
    }
    printf("Uint8 BitsPerPixel: %0d\n", fmt.BitsPerPixel);
    printf("Uint8 BytesPerPixel: %0d\n", fmt.BytesPerPixel);
    printf("Uint8 padding[2]: %0d\n", fmt.padding[2]);
    printf("Uint32 Rmask: %0d\n", fmt.Rmask);
    printf("Uint32 Gmask: %0d\n", fmt.Gmask);
    printf("Uint32 Bmask: %0d\n", fmt.Bmask);
    printf("Uint32 Amask: %0d\n", fmt.Amask);
    printf("Uint8 Rloss: %0d\n", fmt.Rloss);
    printf("Uint8 Gloss: %0d\n", fmt.Gloss);
    printf("Uint8 Bloss: %0d\n", fmt.Bloss);
    printf("Uint8 Aloss: %0d\n", fmt.Aloss);
    printf("Uint8 Rshift: %0d\n", fmt.Rshift);
    printf("Uint8 Gshift: %0d\n", fmt.Gshift);
    printf("Uint8 Bshift: %0d\n", fmt.Bshift);
    printf("Uint8 Ashift: %0d\n", fmt.Ashift);
    printf("int refcount: %0d\n", fmt.refcount);
    printf("struct SDL_PixelFormat *next: %0d\n", fmt.next);
}

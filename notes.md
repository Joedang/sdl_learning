# SDL Learning
## Including SDL
Remember that SDL is included with `#include <SDL2/SDL.h>`, 
so the Lazy Foo tutorials will need to be tweaked in that way.

### SDL2_image
The SDL2_Image documentation is very obnoxious, in that forgets that SDL2 is a thing.
To use SDL2_image, it's required to have `#include <SDL2/SDL_image.h>`.
However it's also necessary to link against it with the `-lSDL2_image` compiler flag.
The documentation fails to mention *what the name of the library to link against is*!

## Documentation
Most of the SDL stuff has man pages associated with it (at least in the Arch package).
The Arch man pages (all SDL man pages?) are severely out of date and incomplete.
Looking at the header files and their comments is a more reliable reference. (`/usr/include/SDL2/`)
Keeping a Vim buffer pointed at the relevant headers also makes auto-completion nicer.
The [SDL wiki](https://wiki.libsdl.org/) is also kind of helpful.

### Tutorials
I'm starting by loosely following [the Lazy Foo tutorial](https://lazyfoo.net/tutorials/SDL/index.php#Hello%20SDL).
I think the coding style they display and the whole "I'm also going to cover C++ basics." thing 
makes it a little difficult to get useful info out of those tutorials.
So, I'm basically collating the stuff they show into a very linear "one of everything" chrestomathy.
I don't care yet about writing the skeleton of a game engine.
I just want a project that serves as a minimum working example of the essentials. 

## Events
Use an object of type `SDL_Event` to buffer events.
Use `SDL_PollEvent(SDL_Event * event)` to grab the events from the user.
The `event` keeps a queue of events received.
Holding keys sends events the way you'd expect from a text editor.
(There's a "down" event on the initial keypress, a pause, and then a bunch of rapid "down" events.)

### Smooth Player Movement
The naive "move when there's a press event" that's shown in the Lazy Foo tutorials gives movement 
that is similar to what you'd experience when holding down a key in a text editor.
To get smoother movement, you have to keep track of whether a key is currently held by looking at keyup and keydown events together.

### Exiting a Program
The `SDL_QUIT` event
`SDL_QUIT` is neccessary and sufficient for:
- exiting a window with "Xing-out" or Alt+F4 via the WM
- SIGINT from the terminal
- maybe more...
This allows the program to close itself gracefully (deallocating resources), 
rather than being killed every time. 

## Drawing with Surfaces
Note that drawing with surfaces is kind of obsolete (CPU), compared to using textures (GPU).
The main reason to use surfaces is if you want to do weird logic that can't be achieved with texture operations.

`SDL_BlitSurface` is used to draw one surface onto another.
(*Blit* is an old-school term for drawing an image.)
Annoyingly, the destination rectangle is only used to convey location, 
and that location gets mutated by `SDL_BlitSurface`.
So, one way to avoid an important rectangle being overwritten is to use an intermediary rectangle.
For example:
```c++
SDL_Rect chrisLocation;// where to draw the test image
    chrisLocation.x = 50;
    chrisLocation.y = 50;
SDL_Rect dstrect_dummy = chrisLocation;// for non-destructive location drawing
/* ... some code ... */
dstrect_dummy = chrisLocation;
SDL_BlitSurface(chris, NULL, windowSurface, &dstrect_dummy);// draw the chris to the screen
```
This would prevent `chrisLocation` from being overwritten, 
which is what would happen if we simply passed `&chrisLocation` into `SDL_BlitSurface`.

### Preconverting Surfaces
When blit-ing a surface, the source is converted to the pixelformat of the target (provided that they differ).
this is a very costly operation, so it makes a lot more sense to do it as infrequently as possible.
For example, converting everything to the same pixelformat when assets are loaded is a good idea. 
```c++
SDL_Surface * windowSurface = NULL;// a surface to be contained by the window
SDL_Surface * mySurface = NULL;// test image
/* ... some code ... */
mySurface = SDL_ConvertSurface(mySurface, windowSurface->format, 0);
```

### Drawing Stretched Surfaces
There's a blit function that scales the source surface according to the width and height of the destination rectangle.
`SDL_BlitScaled(chris, NULL, screenBG, &dstrect_dummy);`
It's worth noting that large surfaces are a relatively costly thing to blit.
If you're drawing an image at the same scale many times, you can save some CPU time by pre-scaling it.

## GPU Rendering
### Textures
The pipeline to get an image shown is this:

```
char * # filename of an image
|
| IMG_Load()
v
Surface # pixel data of the image in RAM
|
| CreateTextureFromSurface() # needs to know the target renderer
v
Texture # pixel data of the image in VRAM
|
| RenderCopy() # blit/draw the texture to the renderer
v
Renderer
|
| RenderPresent() # blit/draw the renderer to the window
v
Window
```

SDL_RenderCopy() actually uses dstrect to scale the texture rather than crop it, unlike BlitSurface().

### Transparency
When directly loading a texture from a file, as opposed to going through a surface first,
you get transparency from the image for free (assuming the image has transparency).
This can be done with `IMG_LoadTexture()`.
You'll want to be able to get the resolution of the image to put into the `dstrect`, so it's scaled proporionally.
This can be done like `SDL_QueryTexture(texturePtr, &format, &access, &rect.w, &rect.h)`.
The function directly copies the width and height values into the rectangle, since it's pass-by-reference.

### Simple Geometry
There are also functions to draw simple geometry with the GPU.
They use whatever color was last set on the renderer with `SDL_SetRenderDrawColor()`.
```c++
SDL_SetRenderDrawColor(renderer, 0x33, 0x11, 0x11, 0xFF);// RGBA, dark red
SDL_RenderClear(renderer);// make the renderer all one color
SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, 0xFF);// gray
drawRect = SDL_Rect {50, 50, 50, 100};// left, top, width, height; rectangle from (50,50) to (100, 150)
SDL_RenderFillRect(renderer, &drawRect);// filled-in rectangle
drawRect = SDL_Rect {20, 20, 100, 100};
SDL_SetRenderDrawColor(renderer, 0x00, 0xCC, 0x00, 0xFF);// green
SDL_RenderDrawRect(renderer, &drawRect);// single-pixel rectangle
SDL_RenderDrawLine(renderer, 20,20,100,150);// x1, y1, x2, y2; line from (20,20) to (100,150)
for (i=0; i < 100; i++) {// dots along a diagonal
    SDL_RenderDrawPoint(renderer, 3*i, 4*i);
}
```

### Animation
My implementation is a little cowboy-ish, the way it juggles data on `boiSrc.w`, but it works.
In a "real" project, I would of course have similar (inheriting?) classes for animated and non-animated sprites,
and I would explicitly keep track of the frame width, number of frames, and reel length.
It's a pretty striaght-forward application of the srcrect argument in `SDL_RenderCopy()`.

It's a little hard to tell if I'm getting screen tearing without the `SDL_RENDERER_PRESENTVSYNC` option disabled.
I tried turning off my compositor and increasing the size of chris, but didn't notice anything.
TODO: I should try it with an excessively high frame rate and `chrisSpeed`.

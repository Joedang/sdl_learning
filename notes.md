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

TODO: I would like to know why keypresses seem to send events as if the program were a text editor, 
      rather than just recognizing down/up events.
      Pressing and holding an arrow key, the image moves one step, waits, and then moves smoothly.

### Exiting a Program
The SDL_QUIT event
SDL_QUIT is neccessary and sufficient for:
- exiting a window with "Xing-out" or Alt+F4 via the WM
- SIGINT from the terminal
- maybe more...

## Drawing to a Location
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

## Smooth Player Movement
The naive ""

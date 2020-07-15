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
This includes both functions and types.
Use `apropos SDL` to get a list of those pages.
Some functions do not have their own man pages. 
Useful comments can usually be found in the header files in `/usr/include/SDL2/` (`grep` helps).
The [SDL wiki](https://wiki.libsdl.org/) is also helpful.

### Tutorials
I'm starting by following [the Lazy Foo tutorial](https://lazyfoo.net/tutorials/SDL/index.php#Hello%20SDL).

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

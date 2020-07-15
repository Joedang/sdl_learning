## Including SDL
Remember that SDL is included with `#include <SDL2/SDL.h>`, 
so the Lazy Foo tutorials will need to be tweaked in that way.

### SDL2
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

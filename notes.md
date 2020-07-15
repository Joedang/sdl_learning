## Including SDL
Remember that SDL is included with `#include <SDL2/SDL.h>`, 
so the Lazy Foo tutorials will need to be tweaked in that way.

## Documentation
Most of the SDL stuff has man pages associated with it (at least in the Arch package).
This includes both functions and types.
Use `apropos SDL` to get a list of those pages.
Some functions do not have their own man pages. 
Useful comments can usually be found in the header files in `/usr/include/SDL2/` (`grep` helps).
The [SDL wiki](https://wiki.libsdl.org/) is also helpful.

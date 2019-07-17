#ifndef DISPLAY_H
#define DISPLAY_H

#include "SDL.h"

class Display {
public:
	bool init();
private:
	SDL_Window *window = nullptr;
	SDL_Renderer *renderer = nullptr;
};

#endif

#ifndef DISPLAY_H
#define DISPLAY_H

#include "SDL.h"
#include "nesem.h"

class Display {
public:
	bool init(State* ctx, int scale_factor);
	void kill();
	bool wait();
	void display(State* ctx);
	void ready();
	void draw_pixel(State* ctx, int x, int y, uint8_t color,
				int bg, uint8_t rc, int prty, uint16_t sprty);
private:
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	void clear_texture(State* ctx);
	void lock_texture(State* ctx);
	void clear();
	uint32_t colors[4];
};

#endif

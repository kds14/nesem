#ifndef DISPLAY_H
#define DISPLAY_H

#include "SDL.h"
#include "nesem.h"

class Display {
private:
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	void clear_texture(State* ctx);
	void lock_texture(State* ctx);
	void clear();
	uint32_t colors[4];
	double rem = 0;
	uint32_t frame_time = 0;
	bool poll(State* ctx);
public:
	bool init(State* ctx, int scale_factor);
	void kill();
	bool wait(State* ctx);
	void display(State* ctx);
	void ready();
	void draw_pixel(State* ctx, int x, int y, uint8_t color);
};

#endif

#include "disp.h"

#define WHITE 255
#define LIGHT_GRAY 160
#define DARK_GRAY 80
#define BLACK 0

const char* WINDOW_TITLE = "nesem";

void Display::clear_texture(State* ctx) {
	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i)
		ctx->pixels[i] = colors[0];
}

void Display::clear() {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
}

void Display::lock_texture(State* ctx) {
	int pitch;
	clear();
	if (ctx->pixels)
		std::copy(ctx->pixels, ctx->pixels + (SCREEN_WIDTH * SCREEN_HEIGHT),
				ctx->prev_pixels);
	SDL_LockTexture(texture, NULL, (void**)&ctx->pixels, &pitch);
	clear_texture(ctx);
}

void Display::draw_pixel(State* ctx, int x, int y, uint8_t color) {
	if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
		return;
	ctx->pixels[SCREEN_WIDTH * y + x] = colors[color];
}

void Display::ready() {
	SDL_UnlockTexture(texture);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_SetRenderTarget(renderer, texture);
}

void Display::display(State* ctx) {
	SDL_RenderPresent(renderer);
	lock_texture(ctx);
}

bool Display::init(State* ctx, int scale_factor) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return false;
	} else {
		renderer = nullptr;
		window = nullptr;
		SDL_CreateWindowAndRenderer(SCREEN_WIDTH * scale_factor, SCREEN_HEIGHT * scale_factor, 0, &window, &renderer);
		if (window == NULL || renderer == NULL) {
			printf( "Window or renderer could not be created! SDL_Error: %s\n", SDL_GetError() );
			return false;
		} else {
			SDL_RenderSetScale(renderer, scale_factor, scale_factor);
			SDL_SetWindowTitle(window, WINDOW_TITLE);
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderClear(renderer);
			texture = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

			SDL_PixelFormat* format = SDL_AllocFormat(SDL_GetWindowPixelFormat(window));

			colors[0] = SDL_MapRGB(format, 84, 84, 84);
			colors[1] = SDL_MapRGB(format, 0, 30, 116);
			colors[2] = SDL_MapRGB(format, 8, 16, 114);
			colors[3] = SDL_MapRGB(format, 48, 0, 136);
			colors[4] = SDL_MapRGB(format, 68, 0, 100);
			colors[5] = SDL_MapRGB(format, 92, 0, 48);
			colors[6] = SDL_MapRGB(format, 84, 4, 0);
			colors[7] = SDL_MapRGB(format, 60, 24, 0);
			colors[8] = SDL_MapRGB(format, 32, 42, 0);
			colors[9] = SDL_MapRGB(format, 8, 58, 0);
			colors[10] = SDL_MapRGB(format, 0, 64, 0);
			colors[11] = SDL_MapRGB(format, 0, 60, 0);
			colors[12] = SDL_MapRGB(format, 0, 50, 60);
			colors[13] = SDL_MapRGB(format, 0, 0, 0);
			colors[14] = SDL_MapRGB(format, 0, 0, 0);
			colors[15] = SDL_MapRGB(format, 0, 0, 0);

			colors[16] = SDL_MapRGB(format, 152, 150, 152);
			colors[17] = SDL_MapRGB(format, 8, 76, 196);
			colors[18] = SDL_MapRGB(format, 48, 50, 236);
			colors[19] = SDL_MapRGB(format, 92, 30, 228);
			colors[20] = SDL_MapRGB(format, 136, 20, 176);
			colors[21] = SDL_MapRGB(format, 160, 20, 100);
			colors[22] = SDL_MapRGB(format, 152, 34, 32);
			colors[23] = SDL_MapRGB(format, 120, 60, 0);
			colors[24] = SDL_MapRGB(format, 84, 90, 0);
			colors[25] = SDL_MapRGB(format, 40, 114, 0);
			colors[26] = SDL_MapRGB(format, 8, 124, 0);
			colors[27] = SDL_MapRGB(format, 0, 118, 40);
			colors[28] = SDL_MapRGB(format, 0, 102, 120);
			colors[29] = SDL_MapRGB(format, 0, 0, 0);
			colors[30] = SDL_MapRGB(format, 0, 0, 0);
			colors[31] = SDL_MapRGB(format, 0, 0, 0);

			colors[32] = SDL_MapRGB(format, 236, 238, 236);
			colors[33] = SDL_MapRGB(format, 76, 154, 236);
			colors[34] = SDL_MapRGB(format, 120, 124, 236);
			colors[35] = SDL_MapRGB(format, 176, 98, 236);
			colors[36] = SDL_MapRGB(format, 228, 84, 236);
			colors[37] = SDL_MapRGB(format, 236, 88, 180);
			colors[38] = SDL_MapRGB(format, 236, 106, 100);
			colors[39] = SDL_MapRGB(format, 212, 136, 32);
			colors[40] = SDL_MapRGB(format, 160, 170, 0);
			colors[41] = SDL_MapRGB(format, 116, 196, 0);
			colors[42] = SDL_MapRGB(format, 76, 208, 32);
			colors[43] = SDL_MapRGB(format, 56, 204, 108);
			colors[44] = SDL_MapRGB(format, 56, 180, 204);
			colors[45] = SDL_MapRGB(format, 60, 60, 60);
			colors[46] = SDL_MapRGB(format, 0, 0, 0);
			colors[47] = SDL_MapRGB(format, 0, 0, 0);

			colors[48] = SDL_MapRGB(format, 236, 238, 236);
			colors[49] = SDL_MapRGB(format, 168, 204, 236);
			colors[50] = SDL_MapRGB(format, 188, 188, 236);
			colors[51] = SDL_MapRGB(format, 212, 178, 236);
			colors[52] = SDL_MapRGB(format, 236, 174, 236);
			colors[53] = SDL_MapRGB(format, 236, 174, 212);
			colors[54] = SDL_MapRGB(format, 236, 180, 176);
			colors[55] = SDL_MapRGB(format, 228, 196, 144);
			colors[56] = SDL_MapRGB(format, 204, 210, 120);
			colors[57] = SDL_MapRGB(format, 180, 222, 120);
			colors[58] = SDL_MapRGB(format, 168, 226, 144);
			colors[59] = SDL_MapRGB(format, 152, 226, 180);
			colors[60] = SDL_MapRGB(format, 160, 214, 228);
			colors[61] = SDL_MapRGB(format, 160, 162, 160);
			colors[62] = SDL_MapRGB(format, 0, 0, 0);
			colors[63] = SDL_MapRGB(format, 0, 0, 0);

			lock_texture(ctx);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			SDL_RenderPresent(renderer);
		}
	}
	return true;
}

void Display::kill() {
	if (renderer)
		SDL_DestroyRenderer(renderer);
	if (window)
		SDL_DestroyWindow(window);
	SDL_Quit();
}

bool Display::poll(State* ctx) {
	SDL_Event event;
	uint8_t inputs = ctx->cpu_mem.last_inputs;
	if (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT)
			return true;
		else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
			uint8_t bits = 0;
			switch (event.key.keysym.sym) {
				case SDLK_RIGHT:
					bits |= 0x80;
					break;
				case SDLK_LEFT:
					bits |= 0x40;
					break;
				case SDLK_DOWN:
					bits |= 0x20;
					break;
				case SDLK_UP:
					bits |= 0x10;
					break;
				case SDLK_RETURN:
				case SDLK_SPACE:
					bits |= 0x08;
					break;
				case SDLK_RSHIFT:
				case SDLK_LSHIFT:
					bits |= 0x04;
					break;
				case SDLK_z:
					bits |= 0x02;
					break;
				case SDLK_x:
					bits |= 0x01;
					break;
				default:
					break;
			}
			if (event.type == SDL_KEYUP) {
				inputs &= ~bits;
			} else {
				inputs |= bits;
			}
		}
	}
	ctx->cpu_mem.last_inputs = inputs;
	return false;
}

bool Display::wait(State* ctx) {
	if (!frame_time) {
		frame_time = SDL_GetTicks();
		return poll(ctx);
	}
	double base = 16.67L - (SDL_GetTicks() - frame_time) + rem;
	int wait = base;
	rem = base - wait;
	if (rem < 0)
		rem = 0;
	if (poll(ctx))
		return true;
	if (wait > 0)
		SDL_Delay(wait);
	frame_time = SDL_GetTicks();
	return poll(ctx);;
}

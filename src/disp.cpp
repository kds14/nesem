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

			colors[0] = SDL_MapRGB(format, WHITE, WHITE, WHITE);
			colors[1] = SDL_MapRGB(format, LIGHT_GRAY, LIGHT_GRAY, LIGHT_GRAY);
			colors[2] = SDL_MapRGB(format, DARK_GRAY, DARK_GRAY, DARK_GRAY);
			colors[3] = SDL_MapRGB(format, BLACK, BLACK, BLACK);

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

bool Display::wait(State* ctx) {
	SDL_Event event;
	double base = 16.67L - SDL_GetTicks() + frame_time + rem;
	int wait = base;
	rem = base - wait;	
	uint8_t inputs = 0;
	if (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT)
			return true;
		else if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_RIGHT:
				inputs |= 0x80;
				break;
			case SDLK_LEFT:
				inputs |= 0x40;
				break;
			case SDLK_DOWN:
				inputs |= 0x20;
				break;
			case SDLK_UP:
				inputs |= 0x10;
				break;
			case SDLK_RETURN:
			case SDLK_SPACE:
				inputs |= 0x08;
				break;
			case SDLK_RSHIFT:
			case SDLK_LSHIFT:
				inputs |= 0x04;
				break;
			case SDLK_z:
				inputs |= 0x02;
				break;
			case SDLK_x:
				inputs |= 0x01;
				break;
			default:
				break;
			}
			ctx->cpu_mem.last_inputs = inputs;
		}
	}
	if (wait > 0)
		SDL_Delay(wait);
	frame_time = SDL_GetTicks();
	return false;
}

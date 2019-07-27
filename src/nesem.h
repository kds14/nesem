#ifndef NESEM_H
#define NESEM_H

#include <memory>
#include <vector>
#include <iostream>
#include "mem.h"
#include <bitset>

const int SCREEN_WIDTH = 256;
const int SCREEN_HEIGHT = 240;

struct State {
	uint8_t a = 0;
	uint8_t x = 0;
	uint8_t y = 0;
	uint16_t pc = 0;
	uint8_t s = 0xFF;
	union {
		uint8_t p = 0;
		struct {
			uint8_t c : 1;
			uint8_t z : 1;
			uint8_t i : 1;
			uint8_t d : 1;
			uint8_t b : 2;
			uint8_t v : 1;
			uint8_t n : 1;
		};
	};
	uint64_t cycles = 0;
	CPU_Memory cpu_mem;
	PPU_Memory ppu_mem;
	uint8_t OAM[0x100];
	uint32_t prev_pixels[SCREEN_WIDTH * SCREEN_HEIGHT];
	uint32_t* pixels = nullptr;
	void print_state() {
		State* ctx = this;
		printf("%04X %02X a: %02X , x: %02X, y: %02X, s: %02X, next: %02X %02X, ", ctx->pc, ctx->cpu_mem.get(ctx->pc), ctx->a, ctx->x, ctx->y, ctx->s, ctx->cpu_mem.get(ctx->pc + 1), ctx->cpu_mem.get(ctx->pc + 2));
		std::cout << std::bitset<8>(ctx->p) << ")" << std::endl;
		std::cout << "OAM:" << std::endl;
		for (int i = 0; i < 0x100; ++i) {
			printf("%02X ", OAM[i]);
			if (!(i % 0xF) && i)
				std::cout << std::endl;
		}
	}
};

static void print_stack(State* ctx) {
	printf("STACK: ");
	for (uint16_t i = ctx->s; i < 0x100; ++i) {
		printf("%02X ", ctx->cpu_mem.get(0x100 + i));
	}
	puts("");
}

#endif

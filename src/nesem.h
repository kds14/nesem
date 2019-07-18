#ifndef NESEM_H
#define NESEM_H

#include <memory>
#include <vector>
#include <iostream>
#include "mem.h"

const int SCREEN_WIDTH = 160;
const int SCREEN_HEIGHT = 144;

struct State {
	uint8_t a = 0;
	uint8_t x = 0;
	uint8_t y = 0;
	uint16_t pc = 0;
	uint8_t s = 0;
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
	uint8_t mem[0x10000];
	uint8_t ppu_mem[0x4000];
	uint32_t prev_pixels[SCREEN_WIDTH * SCREEN_HEIGHT];
	uint32_t pixels[SCREEN_WIDTH * SCREEN_HEIGHT];
	inline uint8_t get_op() { return mem[pc]; };
	inline uint8_t get_b1() { return mem[pc + 1]; };
	inline uint8_t get_b2() { return mem[pc + 2]; };
	inline uint8_t get_16() { return mem[pc + 1]; };
};

#endif

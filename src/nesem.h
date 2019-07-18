#ifndef NESEM_H
#define NESEM_H

#include <memory>
#include <vector>

class Memory;
class CPU;
class PPU;
class Display;

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
	std::unique_ptr<Memory> mem;
	std::unique_ptr<Memory> ppu_mem;
	std::unique_ptr<CPU> cpu;
	std::unique_ptr<PPU> ppu;
	std::unique_ptr<Display> disp;
	State();
};

#endif

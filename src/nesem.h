#ifndef NESEM_H
#define NESEM_H

#include <memory>
#include <vector>
#include "mem.h"

struct state {
	uint8_t a;
	uint8_t x;
	uint8_t y;
	uint16_t pc;
	uint8_t s;
	union {
		uint8_t p;
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
	std::unique_ptr<Memory> mem;
	uint64_t cycles;
};

#endif

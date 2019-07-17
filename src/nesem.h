#ifndef NESEM_H
#define NESEM_H

#include <memory>
#include <vector>

struct state {
	uint8_t a;
	uint8_t x;
	uint8_t y;
	uint16_t pc;
	uint8_t s;
	uint8_t p;
	std::unique_ptr<uint8_t> mem;
};

#endif

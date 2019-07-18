#ifndef PPU_H
#define PPU_H

#include "nesem.h"

class PPU {
private:
	Memory* mem;
public:
	void handle(uint64_t cycles);
	PPU(Memory* m) : mem(m) {}
};

#endif

#ifndef MEM_H
#define MEM_H

#include <memory>
#include "nesem.h"

namespace Memory {
	enum MemType {Main, PPU};

	inline void dset(uint8_t* mem, uint16_t addr, uint8_t val) { mem[addr] = val; }
	inline void dset(uint8_t* mem, uint16_t addr, uint8_t* buf, size_t len) {
		std::copy(buf, buf + len, &mem[addr]);
	}
	inline uint8_t dget(uint8_t* mem, uint16_t addr) { return mem[addr]; }
	inline uint16_t dget16(uint8_t* mem, uint16_t addr) {
		return ((uint16_t*)mem)[addr];
	}

	uint8_t get(uint8_t mem, MemType t, uint16_t addr);
	void get(uint8_t mem, MemType t, uint16_t addr, uint8_t* buf, size_t len);
	void set(uint8_t mem, MemType t, uint16_t addr, uint8_t val);
	void set(uint8_t mem, MemType t, uint16_t addr, uint8_t* buf, size_t len);
};

#endif

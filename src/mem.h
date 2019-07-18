#ifndef MEM_H
#define MEM_H

#include <memory>

class Memory {
private:
	std::unique_ptr<uint8_t[]> mem;
public:
	inline void dset(uint16_t addr, uint8_t val) { mem[addr] = val; }
	inline void dset(uint16_t addr, uint8_t* buf, size_t len) {
		std::copy(buf, buf + len, &mem[addr]);
	}
	void set(uint16_t addr, uint8_t val);
	void set(uint16_t addr, uint8_t* buf, size_t len);
	inline uint8_t dget(uint16_t addr) { return mem[addr]; }
	inline uint16_t dget16(uint16_t addr) {
		return ((uint16_t*)mem.get())[addr];
	}
	uint8_t get(uint16_t addr);
	void get(uint16_t addr, uint8_t* buf, size_t len);
	Memory(uint32_t size) {
		mem = std::make_unique<uint8_t[]>(size);
	};
};

#endif

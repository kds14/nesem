#ifndef MEM_H
#define MEM_H

#include <memory>

class Memory {
private:
	std::unique_ptr<uint8_t[]> mem;
public:
	void set_mem(uint16_t addr, uint8_t val);
	void set_mem(uint16_t addr, uint8_t* buf, size_t len);
	uint8_t get_mem(uint16_t addr);
	void get_mem(uint16_t addr, uint8_t* buf, size_t len);
	Memory(uint32_t size) {
		mem = std::make_unique<uint8_t[]>(size);
	};
};

#endif

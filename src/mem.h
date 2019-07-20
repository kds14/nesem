#ifndef MEM_H
#define MEM_H

#include <memory>
#include <string>

const size_t CPU_MEM_SIZE = 0x10000;
const size_t PPU_MEM_SIZE = 0x4000;

class MemRegion {
private:
	bool readonly;
	size_t sz;
	std::unique_ptr<uint8_t[]> mem;
public:
	MemRegion(bool readonly, size_t sz) : readonly(readonly), sz(sz),
		mem(std::unique_ptr<uint8_t[]>(new uint8_t[sz])) { };
	inline bool isROM() { return readonly; };
	inline size_t size() { return sz; };
	inline void dset(uint16_t addr, uint8_t val) { mem[addr] = val; }
	inline void dset16(uint16_t addr, uint8_t* buf, size_t len) {
		std::copy(buf, buf + len, &mem[addr]);
	}
	inline uint8_t dget(uint16_t addr) { return mem[addr]; }
	inline uint16_t dget16(uint16_t addr) {
		return (((uint16_t)dget(addr + 1)) << 8) | dget(addr);
	}
};

class Memory {
public:
	virtual uint8_t get(uint16_t addr) = 0;
	virtual void set(uint8_t val, uint16_t addr) = 0;
	uint16_t get16(uint16_t addr) {
		return (((uint16_t)get(addr + 1)) << 8) | get(addr);
	};
	void set16(uint16_t val, uint16_t addr) {
		set(val & 0xFF, addr);
		set(val >> 8, addr + 1);
	};
	virtual ~Memory() {};
};

class CPU_Memory : public Memory {
private:
	MemRegion internal_RAM;
	MemRegion PPU_regs;
	MemRegion APU_IO_regs;
	MemRegion APU_IO_func;
	MemRegion cartridge;
public:
	uint8_t get(uint16_t addr);
	void set(uint8_t val, uint16_t addr);
	CPU_Memory();
};

class PPU_Memory : public Memory {
private:
	MemRegion ptable0;
	MemRegion ptable1;
	MemRegion ntable0;
	MemRegion ntable1;
	MemRegion ntable2;
	MemRegion ntable3;
	MemRegion palette_ram_idx;
public:
	uint8_t get(uint16_t addr);
	void set(uint8_t val, uint16_t addr);
	PPU_Memory();
};

void file2mem(std::string filename, CPU_Memory** cpu_mem, PPU_Memory** ppu_mem);
#endif

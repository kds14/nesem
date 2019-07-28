#ifndef MEM_H
#define MEM_H

#include <memory>
#include <string>

const size_t CPU_MEM_SIZE = 0x10000;
const size_t PPU_MEM_SIZE = 0x4000;

struct State;

class MemRegion {
private:
	bool readonly;
	uint16_t sz;
	std::unique_ptr<uint8_t[]> mem;
	uint16_t start;
public:
	MemRegion(bool readonly, size_t sz, uint8_t* buff, uint16_t start) :
		readonly(readonly), sz(sz), mem(std::unique_ptr<uint8_t[]>(buff)),
		start(start) { };
	MemRegion(bool readonly, size_t sz, uint16_t start) : readonly(readonly), sz(sz),
		mem(std::unique_ptr<uint8_t[]>(new uint8_t[sz]())), start(start) { };
	MemRegion() : readonly(false), sz(0), start(0) {};
	inline bool isROM() { return readonly; };
	inline uint16_t size() { return sz; };
	inline uint16_t get_start() { return start; };
	inline uint16_t get_end() { return start + sz; };
	inline void dset(uint16_t addr, uint8_t val) { mem[addr] = val; }
	inline void set(uint16_t addr, uint8_t val) { mem[(addr - start) % sz] = val; }
	inline uint8_t get(uint16_t addr) { return mem[(addr - start) % sz]; }
	inline void dset16(uint16_t addr, uint8_t* buf, size_t len) {
		std::copy(buf, buf + len, &mem[addr]);
	}
	inline uint8_t dget(uint16_t addr) { return mem[addr]; }
	inline uint16_t dget16(uint16_t addr) {
		return (((uint16_t)dget(addr + 1)) << 8) | dget(addr);
	}
	inline uint8_t* get_ptr(uint16_t addr) { return &mem[addr]; }
	inline uint8_t* get_ptr_auto(uint16_t addr) { return &mem[(addr - start) % sz]; }
	void print();
};

class Memory {
private:
	virtual MemRegion* get_mem_region(uint16_t addr) = 0;
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

class PPU_Memory : public Memory {
private:
	MemRegion ptable;
	MemRegion ntable0;
	MemRegion ntable1;
	MemRegion ntable2;
	MemRegion ntable3;
	MemRegion palette_ram_idx;
	MemRegion* get_mem_region(uint16_t addr);
public:
	uint8_t get(uint16_t addr);
	void set(uint8_t val, uint16_t addr);
	PPU_Memory(uint8_t* chr_buff);
	PPU_Memory() {};
	void print();
};

class CPU_Memory : public Memory {
private:
	MemRegion internal_RAM;
	MemRegion PPU_regs;
	MemRegion APU_IO_regs;
	MemRegion APU_IO_func;
	MemRegion cartridge;
	MemRegion work_ram;
	int prg_cnt;
	void DMA(uint8_t high_byte);
	MemRegion* get_mem_region(uint16_t addr);
	uint8_t* OAM;
	void DMA_inc(uint8_t val);
	void DATA_inc(uint8_t val);
	uint64_t* cycles;
	uint16_t ppuaddr_latch = 0;
	PPU_Memory* ppu_mem;
	uint8_t input_latch_shift = 0;
	uint8_t ctrl_shift_strobe = 0;
	uint8_t input_latch = 0;
public:
	uint8_t get(uint16_t addr);
	void set(uint8_t val, uint16_t addr);
	CPU_Memory(uint64_t* cycles, uint8_t* buff, int prg_cnt, uint8_t* OAM,
				PPU_Memory* ppu_mem);
	CPU_Memory() {};
	uint8_t* get_PPU_regs();
	bool nmi = false;
	bool nmi_output = false;
	uint8_t last_inputs = 0;
};


#endif

#include "mem.h"
#include <iostream>

CPU_Memory::CPU_Memory(uint8_t* buff, int prg_cnt, uint8_t* OAM) : internal_RAM(false, 0x800, 0),
	PPU_regs(false, 0x8, 0x2000), APU_IO_regs(false, 0x18, 0x4000),
	APU_IO_func(false, 0x8, 0x4018), work_ram(true, 0x2000, 0x6000), prg_cnt(prg_cnt),
 	cartridge(false, prg_cnt * 0x4000, buff + 0x10, 0x8000), OAM(OAM) {}

PPU_Memory::PPU_Memory() : ptable0(false, 0x1000, 0), ptable1(false, 0x1000, 0x1000),
	ntable0(false, 0x400, 0x2000), ntable1(false, 0x400, 0x2400),
	ntable2(false, 0x400, 0x2800), ntable3(false, 0x400, 0x2C00),
	palette_ram_idx(true, 0x0020, 0x3F00) { }

MemRegion* CPU_Memory::get_mem_region(uint16_t addr) {
	if (addr < 0x2000) {
		return &internal_RAM;
	} else if (addr < 0x4000) {
		return &PPU_regs;
	} else if (addr < 0x4018) {
		return &APU_IO_regs;
	} else if (addr < 0x4020) {
		return &APU_IO_func;
	} else if (addr < 0x8000) {
		return &work_ram;
	} else {
		return &cartridge;
	}
}

void CPU_Memory::DMA(uint8_t high_byte) {
	//std::cout << "DMA" << std::endl;
	uint16_t addr = (uint16_t)high_byte << 8;
	MemRegion* mr;
	uint8_t extra = 0;
	do {
		mr = get_mem_region(addr);
		uint16_t sz = std::min(mr->get_end() - addr, 0x100);
		uint8_t* begin = mr->get_ptr(addr);
		std::copy(begin, begin + 0x100, OAM + extra);
		addr += sz;
		extra += sz;
		
	} while (extra < 0x100 && mr->get_end() < addr | 0xFF);
}

uint8_t CPU_Memory:: get(uint16_t addr) {
	MemRegion* mr = get_mem_region(addr);
	uint8_t val =  mr->dget((addr - mr->get_start()) % mr->size());
	if (addr = 0x2002) {
		mr->dset(addr, (val & 0x80) | (nmi << 7));
		nmi = false;
	}
	return val;
}

void CPU_Memory::DMA_inc(uint8_t val) {
	MemRegion* mr = get_mem_region(0x2000);
	uint8_t PPUCTRL = mr->dget(0x2000);
	uint8_t* OAMADDR = mr->get_ptr(0x2003);
	OAM[*OAMADDR] = val;
	*OAMADDR += (PPUCTRL & 0x4) * 31 + 1;
}

void CPU_Memory::set(uint8_t val, uint16_t addr) {
	MemRegion* mr = get_mem_region(addr);
	if (addr == 0x2004) {
		// TODO OAMDATA
		//std::cout << "OAMDATA " << (int)val << std::endl;
		DMA_inc(val);
	} else if (addr == 0x4014) {
		// TODO OAMDMA
		DMA(val);
	} else {
		mr->dset((addr - mr->get_start()) % mr->size(), val);
	}
	if (addr == 0x2000) {
		nmi_output = val >> 7;
	}

}

uint8_t* CPU_Memory::get_PPU_regs() {
	return PPU_regs.get_ptr(0);
}

uint8_t PPU_Memory::get(uint16_t addr) {
	return 0;
}

void PPU_Memory::set(uint8_t val, uint16_t addr) {
}

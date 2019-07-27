#include "mem.h"
#include <iostream>

void MemRegion::print() {
	printf("%04X: ", start);
	for (uint16_t i = 0; i < sz; ++i) {
		printf("%02X ", mem[i]);
		if (i && !(i % 16) && i != sz - 1)
			printf("\n%04X: ", start + i);
	}
	puts("");
}

CPU_Memory::CPU_Memory(uint64_t* cycles, uint8_t* buff, int prg_cnt,
						uint8_t* OAM, PPU_Memory* ppu_mem) :
	internal_RAM(false, 0x800, 0), cycles(cycles),
	PPU_regs(false, 0x8, 0x2000), APU_IO_regs(false, 0x18, 0x4000),
	APU_IO_func(false, 0x8, 0x4018), work_ram(true, 0x2000, 0x6000), prg_cnt(prg_cnt),
 	cartridge(false, prg_cnt * 0x4000, buff + 0x10, 0x8000), OAM(OAM) {
	this->ppu_mem = ppu_mem;
}

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
	uint16_t addr = (uint16_t)high_byte << 8;
	MemRegion* mr;
	uint8_t extra = 0;
	for (uint16_t i = 0x0; i < 0x100; ++i) {
		mr = get_mem_region(addr + i);
		OAM[i] = mr->dget((addr + i - mr->get_start()) % mr->size());
	}
	*cycles += 513;
}

uint8_t CPU_Memory:: get(uint16_t addr) {
	MemRegion* mr = get_mem_region(addr);
	uint8_t val =  mr->dget((addr - mr->get_start()) % mr->size());
	if (addr == 0x2002) {
		mr->dset((addr - mr->get_start()) % mr->size(), (val & 0x7F));
		nmi = false;
	} else if (addr == 0x2006) {
		ppuaddr_latch = 0;
	} else if (addr == 0x2007) {
		DATA_inc(val);
	}
	return val;
}

void CPU_Memory::DMA_inc(uint8_t val) {
	MemRegion* mr = get_mem_region(0x2000);
	uint8_t PPUCTRL = mr->get(0x2000);
	uint8_t* OAMADDR = mr->get_ptr_auto(0x2003);
	OAM[*OAMADDR] = val;
	*OAMADDR += 1;
}

void CPU_Memory::DATA_inc(uint8_t val) {
	auto reg_mr = get_mem_region(0x2000);
	uint8_t PPUCTRL = reg_mr->get(0x2000);
	ppu_mem->set(ppuaddr_latch, val);
	ppuaddr_latch += (PPUCTRL & 0x4) * 31 + 1;
}

void CPU_Memory::set(uint8_t val, uint16_t addr) {
	MemRegion* mr = get_mem_region(addr);
	if (addr == 0x2004) {
		// TODO OAMDATA
		//std::cout << "OAMDATA " << (int)val << std::endl;
		DMA_inc(val);
	} else if (addr == 0x2006) {
		ppuaddr_latch = (ppuaddr_latch << 8) | val;
	} else if (addr == 0x4014) {
		// TODO OAMDMA
		DMA(val);
	} else {
		mr->dset((addr - mr->get_start()) % mr->size(), val);
	}
	// also
	if (addr == 0x2000) {
		nmi_output = val >> 7;
	} else if (addr == 0x2007) {
		DATA_inc(val);
	}

}

uint8_t* CPU_Memory::get_PPU_regs() {
	return PPU_regs.get_ptr(0);
}

MemRegion* PPU_Memory::get_mem_region(uint16_t addr) {
	if (addr < 0x1000) {
		return &ptable0;
	} else if (addr < 0x2000) {
		return &ptable1;
	} else if (addr < 0x2400) {
		return &ntable0;
	} else if (addr < 0x2800) {
		return &ntable1;
	} else if (addr < 0x2C00) {
		return &ntable2;
	} else if (addr < 0x3000) {
		return &ntable2;
	} else if (addr < 0x3400) {
		return &ntable0;
	} else if (addr < 0x3800) {
		return &ntable1;
	} else if (addr < 0x3C00) {
		return &ntable2;
	} else if (addr < 0x3F00) {
		return &ntable3;
	} else if (addr < 0x4000) {
		return &palette_ram_idx;
	} else {
		return nullptr;
	}
}

uint8_t PPU_Memory::get(uint16_t addr) {
	auto mr = get_mem_region(addr);
	return mr->dget((addr - mr->get_start()) % mr->size());
}

void PPU_Memory::set(uint8_t val, uint16_t addr) {
	auto mr = get_mem_region(addr);
	mr->dset((addr - mr->get_start()) % mr->size(), val);
}

void PPU_Memory::print() {
	puts("ptable0");
	ptable0.print();
	puts("ptable1");
	ptable1.print();
	puts("ntable0");
	ntable0.print();
	puts("ntable1");
	ntable1.print();
	puts("ntable2");
	ntable2.print();
	puts("ntable3");
	ntable3.print();
}

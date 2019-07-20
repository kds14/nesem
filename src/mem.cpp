#include "mem.h"

CPU_Memory::CPU_Memory() : internal_RAM(false, 0x800), PPU_regs(false, 0x8), APU_IO_regs(false, 0x18), APU_IO_func(false, 0x8), cartridge(true, 0xBFE0) {
}

PPU_Memory::PPU_Memory() : ptable0(false, 0x1000), ptable1(false, 0x1000), ntable0(false, 0x400), ntable1(false, 0x400), ntable2(false, 0x400), ntable3(false, 0x400), palette_ram_idx(true, 0x0020) {
}

uint8_t CPU_Memory:: get(uint16_t addr) {
	if (addr < 0x2000) {
		return internal_RAM.dget(addr % 0x800);
	} else if (addr < 0x4000) {
		return PPU_regs.dget((addr - 0x2000) % 0x8);
	} else if (addr < 0x4018) {
		return APU_IO_regs.dget((addr - 0x4000));
	} else if (addr < 0x4020) {
		return APU_IO_func.dget((addr - 0x4018));
	} else {
		return cartridge.dget((addr - 0x4020));
	}

}
void CPU_Memory::set(uint8_t val, uint16_t addr) {
	if (addr < 0x2000) {
		return internal_RAM.dset(addr % 0x800, val);
	} else if (addr < 0x4000) {
		return PPU_regs.dset((addr - 0x2000) % 0x8, val);
	} else if (addr < 0x4018) {
		return APU_IO_regs.dset((addr - 0x4000), val);
	} else if (addr < 0x4020) {
		return APU_IO_func.dset((addr - 0x4018), val);
	} else {
		return cartridge.dset((addr - 0x4020), val);
	}

}

uint8_t PPU_Memory::get(uint16_t addr) {
	return 0;
}

void PPU_Memory::set(uint8_t val, uint16_t addr) {
}

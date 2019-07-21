#include "mem.h"

CPU_Memory::CPU_Memory(uint8_t* buff, int prg_cnt) : internal_RAM(false, 0x800), PPU_regs(false, 0x8), APU_IO_regs(false, 0x18), APU_IO_func(false, 0x8), cartridge(false, prg_cnt * 0x4000, buff + 0x10), work_ram(true, 0x2000), prg_cnt(prg_cnt) {
	
}

CPU_Memory::CPU_Memory() : internal_RAM(false, 0x800), PPU_regs(false, 0x8), APU_IO_regs(false, 0x18), APU_IO_func(false, 0x8), work_ram(true, 0x2000), cartridge(false, 0x0) {
	
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
	} else if (addr < 0x8000) {
		return cartridge.dget((addr - 0x6000) % (0x2000));
	} else {
		return cartridge.dget((addr - 0x8000) % (0x4000 * prg_cnt));
	}

}
void CPU_Memory::set(uint8_t val, uint16_t addr) {
	if (addr < 0x2000) {
		internal_RAM.dset(addr % 0x800, val);
	} else if (addr < 0x4000) {
		PPU_regs.dset((addr - 0x2000) % 0x8, val);
	} else if (addr < 0x4018) {
		APU_IO_regs.dset((addr - 0x4000), val);
	} else if (addr < 0x4020) {
		APU_IO_func.dset((addr - 0x4018), val);
	} else if (addr < 0x8000) {
		return cartridge.dset((addr - 0x6000) % 0x2000, val);
	} else {
		cartridge.dset((addr - 0x8000) % (0x4000 * prg_cnt), val);
	}

}

uint8_t PPU_Memory::get(uint16_t addr) {
	return 0;
}

void PPU_Memory::set(uint8_t val, uint16_t addr) {
}

#include "ppu.h"

void PPU::tick(State* ctx) {
}

uint16_t PPU::PPUCTRL_nametable_addr() {
	return (((*PPUCTRL) & 0x3)) * 0x400 + 0x2000;
}

uint8_t PPU::PPUCTRL_VRAM_inc() {
	return (((*PPUCTRL) >> 2) & 0x1) * 31 + 1;
}

uint16_t PPU::PPUCTRL_obj_table_addr() {
	return (((*PPUCTRL) >> 3) & 0x1) * 0x1000;
}

uint16_t PPU::PPUCTRL_bg_table_addr() {
	return (((*PPUCTRL) >> 4) & 0x1) * 0x1000;
}

uint8_t PPU::PPUCTRL_obj_size() {
	return (((*PPUCTRL) >> 5) & 0x1) * 0x8 + 0x8;
}

bool PPU::PPUCTRL_master_slave() {
	return (((*PPUCTRL) >> 6) & 0x1);
}

bool PPU::PPUCTRL_vblank_nmi() {
	return (((*PPUCTRL) >> 7) & 0x1);
}

PPU::PPU(uint8_t* register_start) {
	PPUCTRL = register_start;
	PPUMASK = register_start + 1;
	PPUSTATUS = register_start + 2;
	PPUSCROLL = register_start + 5;
	PPUADDR = register_start + 6;
	PPUDATA = register_start + 7;
}

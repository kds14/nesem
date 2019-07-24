#ifndef PPU_H
#define PPU_H

#include "nesem.h"

class PPU {
private:
	uint8_t* PPUCTRL;
	uint8_t* PPUMASK;
	uint8_t* PPUSTATUS;
	uint8_t* PPUSCROLL;
	uint8_t* PPUADDR;
	uint8_t* PPUDATA;

	uint8_t PPUCTRL_VRAM_inc();
	uint16_t PPUCTRL_nametable_addr();
	uint16_t PPUCTRL_obj_table_addr();
	uint16_t PPUCTRL_bg_table_addr();
	uint8_t PPUCTRL_obj_size();
	bool PPUCTRL_master_slave();
	bool PPUCTRL_vblank_nmi();
public:
	void tick(State* ctx);
	PPU(uint8_t* register_start);
};

#endif

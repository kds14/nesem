#ifndef PPU_H
#define PPU_H

#include "nesem.h"
#include "disp.h"

class PPU {
private:
	static const uint64_t frame_dots = 89342;
	static const int scanline_dots = 341;
	static const int hblank_dots = 85;

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

	void set_vblank(bool set);
	bool PPUCTRL_master_slave();
	bool PPUCTRL_vblank_nmi();
	uint64_t ppu_cycles;
	Display* disp;
	int scanline;
	uint16_t dot;
	bool draw_3dots(State* ctx);
	void draw_bg(State* ctx, uint8_t x, uint8_t y);
	void draw_obj(State* ctx, uint8_t x, uint8_t y);
	void draw_grid(State* ctx, uint8_t x, uint8_t y);
	//void draw_scanline(State* ctx, uint8_t y);
	uint8_t get_col_bit(State* ctx, uint8_t idx, uint8_t y, uint8_t x, uint8_t bit, bool bg);
public:
	bool tick(State* ctx);
	PPU(uint8_t* register_start, Display* disp);
};

#endif

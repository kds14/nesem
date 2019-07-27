#include "ppu.h"
//#define PPU_DBG

uint8_t PPU::get_col_bit(State* ctx, uint8_t idx, uint8_t y, uint8_t x, bool bit, bool bg) {
	uint16_t a = bg ? PPUCTRL_bg_table_addr() : PPUCTRL_obj_table_addr();
	uint16_t addr = (a | (((idx << 1) | bit) << 3) | y);
	uint8_t val = ctx->ppu_mem.get(addr);
	if (val)
		printf("%02X\n", val);
	return (val >> (7 - x)) & 0x1;
}

void PPU::draw_bg (State* ctx, uint8_t x, uint8_t y) {
	uint8_t p = ctx->ppu_mem.get(PPUCTRL_nametable_addr() + (y/8) * 32 + (x/8));
	if (p)
		printf("DRAWING BG PIXEL (%02X, %02X) %02X \n", x, y, p);
	uint8_t color, x_fine, y_fine;
	x_fine = x % 8;
	y_fine = y % 8;
	color = (get_col_bit(ctx, p, y_fine, x_fine, 1, true) << 1)
			| get_col_bit(ctx, p, y_fine, x_fine, 0, true);
	if (color)
		disp->draw_pixel(ctx, x, y, color);
}

void PPU::draw_obj (State* ctx, uint8_t x, uint8_t y) {
#ifdef PPU_DBG
	printf("DRAWING PIXEL (%02X, %02X)\n", x, y);
#endif
	uint8_t color, y_byte, x_byte, idx_byte, x_fine, y_fine;
	for (int i = 0; i < 64; ++i) {
		y_byte = ctx->OAM[i * 4];
		x_byte = ctx->OAM[i * 4 + 3];
		idx_byte = ctx->OAM[i * 4 + 1];
		x_fine = x - x_byte;
		y_fine = y - y_byte;
#ifdef PPU_DBG
		if (y_byte || x_byte || idx_byte)
			printf("SPRITE %d (%02X %02X %02X)\n", i, y_byte, x_byte, idx_byte);
#endif
		if (x_fine <= PPUCTRL_obj_size() && y_fine <= PPUCTRL_obj_size()) {
			color = (get_col_bit(ctx, idx_byte, y_fine, x_fine, 1, false) << 1)
				| get_col_bit(ctx, idx_byte, y_fine, x_fine, 0, false);
			disp->draw_pixel(ctx, x, y, color);
		}
	}
}

void PPU::draw_3dots(State* ctx) {
	uint16_t x, y;
	for (int i = 0; i < 3; ++i) {
		y = (uint16_t)scanline;
		x = dot++;
		if (scanline == -1 && dot == 1) {
			ctx->cpu_mem.nmi = false;
			*PPUSTATUS &= 0x7F;
		} else if (scanline == 241 && dot == 1) {
			ctx->cpu_mem.nmi = true;
			*PPUSTATUS |= 0x80;
		} else if (scanline > 240) {
		} else if (scanline > -1 && dot <= scanline_dots) {
			draw_bg(ctx, x, y);
			draw_obj(ctx, x, y);
		}
		if (dot >= scanline_dots) {
			dot = 0;
			scanline = (int)(scanline + 1) >= 262 ? -1 : scanline + 1;
			//draw_scanline(ctx, y);
		}
#ifdef PPU_DBG
		std::cout << "SCANLINE: " << (int)scanline << std::endl;
#endif
	}
}

bool PPU::tick(State* ctx) {
	ppu_cycles += 3;
	draw_3dots(ctx);
	if (ppu_cycles >= frame_dots) {
		//puts("TICK");
		ppu_cycles = ppu_cycles % frame_dots;
		//disp->draw_pixel(ctx, 50, 50, 2);
		disp->ready();
		disp->display(ctx);
		if (disp->wait())
			return true;
	}
	return false;
}

void PPU::set_vblank(bool set) {
	if (set)
		*PPUSTATUS |= 0x80;
	else
		*PPUSTATUS &= 0x7F;
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

PPU::PPU(uint8_t* register_start, Display* disp) : disp(disp) {
	PPUCTRL = register_start;
	PPUMASK = register_start + 1;
	PPUSTATUS = register_start + 2;
	PPUSCROLL = register_start + 5;
	PPUADDR = register_start + 6;
	PPUDATA = register_start + 7;
	ppu_cycles = 0;
	scanline = -1;
	dot = 0;
}

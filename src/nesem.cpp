#include <iostream>
#include "nesem.h"
#include "cpu.h"
#include "mem.h"
#include "ppu.h"
#include "disp.h"

#include <iostream>
#include <fstream>

static void file2mem(State* ctx, std::string filename) {
	std::ifstream f(filename, std::ios::binary);
	if (!f) {
		std::cout << "Failed to open file" << std::endl;
		exit(0);
	}
	f.seekg(0, std::ios::end);
	std::streamsize sz = f.tellg();
	f.seekg(0, std::ios::beg);
	uint8_t* header = new uint8_t[16]();
	if (!f.read((char*)header, 16)) {
		std::cout << "Failed to read header" << std::endl;
		exit(0);
	}
	uint16_t prg_rom_size = header[4] * 16384;
	uint16_t chr_rom_size = header[5] * 8192;
	uint8_t mapper = (header[7] & 0xF0) | (header[6] >> 4);
	uint8_t* prg_buff = new uint8_t[prg_rom_size]();
	if (!f.read((char*)prg_buff, prg_rom_size)) {
		std::cout << "Failed to read prg" << std::endl;
		exit(0);
	}
	uint8_t* chr_buff = new uint8_t[chr_rom_size]();
	if (!f.read((char*)chr_buff, chr_rom_size)) {
		std::cout << "Failed to read chr" << std::endl;
		exit(0);
	}
	f.close();
	ctx->ppu_mem = PPU_Memory(chr_buff);
	ctx->cpu_mem = CPU_Memory(&ctx->cycles, prg_buff, header[4],
			ctx->OAM, &ctx->ppu_mem);
	free(header);
}

int main(int argc, char** argv) {
	std::unique_ptr<State> ctx = std::make_unique<State>();
	std::unique_ptr<Display> disp = std::make_unique<Display>();
	file2mem(ctx.get(), argv[1]);
	std::unique_ptr<CPU> cpu = std::make_unique<CPU>();
	disp->init(ctx.get(), 2);
	std::unique_ptr<PPU> ppu = std::make_unique<PPU>(ctx->cpu_mem.get_PPU_regs(), disp.get());

	ctx->pc = ctx->cpu_mem.get16(0xFFFC);
	ctx->p = 0x34;
	ctx->s = 0xFD;
	uint64_t prev_cycles = 0;
	while (1) {
		prev_cycles = ctx->cycles;
		cpu->fde(ctx.get());
		for (int i = 0; i <  ctx->cycles - prev_cycles; ++i) {
			if (ppu->tick(ctx.get()))
				goto nesem_end;
		}
	}
nesem_end:
	disp->kill();
	return 0;
}

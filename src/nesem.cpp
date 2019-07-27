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
	f.seekg(0, std::ios::end);
	std::streamsize sz = f.tellg();
	f.seekg(0, std::ios::beg);
	uint8_t* buff = new uint8_t[sz]();
	if (!f.read((char*)buff, sz)) {
		std::cout << "Failed to read file" << std::endl;
		exit(0);
	}
	f.close();
	printf("%02X\n", buff[4]);
	ctx->cpu_mem = CPU_Memory(buff, buff[4], ctx->OAM);
	ctx->ppu_mem = PPU_Memory();
}

int main(int argc, char** argv) {
	std::unique_ptr<State> ctx = std::make_unique<State>();
	std::unique_ptr<Display> disp = std::make_unique<Display>();
	file2mem(ctx.get(), argv[1]);
	std::unique_ptr<CPU> cpu = std::make_unique<CPU>();
	disp->init(ctx.get(), 1);
	std::unique_ptr<PPU> ppu = std::make_unique<PPU>(ctx->cpu_mem.get_PPU_regs(), disp.get());

	ctx->pc = ctx->cpu_mem.get16(0xFFFC);
	ctx->p = 0x34;
	ctx->s = 0xFD;
	ctx->print_state();
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

#include <iostream>
#include "nesem.h"
#include "cpu.h"
#include "mem.h"
#include "ppu.h"
#include "disp.h"

#include <iostream>
#include <fstream>

static void file2mem(std::string filename, CPU_Memory* cpu_mem, PPU_Memory* ppu_mem) {
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
	*cpu_mem = CPU_Memory(buff, buff[4]);
}

int main(int argc, char** argv) {
	std::unique_ptr<State> ctx = std::make_unique<State>();
	std::unique_ptr<Display> disp = std::make_unique<Display>();
	std::unique_ptr<CPU> cpu = std::make_unique<CPU>();
	file2mem(argv[1], &ctx->cpu_mem, nullptr);

	ctx->pc = ctx->cpu_mem.get16(0xFFFC);
	ctx->p = 0x34;
	ctx->s = 0xFD;
	
	disp->init(ctx.get(), 1);
	while (disp->wait()) { cpu->fde(ctx.get());}
	disp->kill();
	return 0;
}

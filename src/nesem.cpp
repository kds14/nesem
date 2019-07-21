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
	std::cout << (int)sz << std:: endl;
	f.seekg(0, std::ios::beg);
	uint8_t* buff = new uint8_t[sz]();
	if (!f.read((char*)buff, sz)) {
		std::cout << "Failed to read file" << std::endl;
		exit(0);
	}
	for (size_t i = 0; i < sz; ++i) {
		if (!(i % 16)) {
			std::cout << std::endl << std::hex << (unsigned int)i << "\t";
		}
		printf("%02X ", buff[i]);
	}
	std::cout << sz << std::endl;
	f.close();
	*cpu_mem = CPU_Memory(buff);
}

int main(int argc, char** argv) {
	std::unique_ptr<State> ctx = std::make_unique<State>();
	std::unique_ptr<Display> disp = std::make_unique<Display>();
	std::unique_ptr<CPU> cpu = std::make_unique<CPU>();
	std::cout << argv[1] << std::endl;
	file2mem(argv[1], &ctx->cpu_mem, nullptr);
	std::cout << 2 << std::endl;

	ctx->cpu_mem.set(0x10, 0x5);
	ctx->cpu_mem.set(0x15, 0x6);
	ctx->cpu_mem.set(0x7, 0x1510);
	ctx->cpu_mem.set(0x42, 0x1512);
	ctx->a = 3;
	ctx->pc = 0x2000;
	ctx->p = 0xFF;
	ctx->x = 0x3;
	ctx->y = 0x4;
	ctx->cpu_mem.set(0xA1, 0x2000);
	ctx->cpu_mem.set(0x02, 0x2001);
	
	disp->init(ctx.get(), 1);
	while (disp->wait()) { cpu->fde(ctx.get());}
	disp->kill();
	return 0;
}

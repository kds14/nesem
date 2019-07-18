#include <iostream>
#include "nesem.h"
#include "cpu.h"
#include "mem.h"
#include "ppu.h"
#include "disp.h"

State::State() : mem (std::make_unique<Memory>(0x10000)), ppu_mem (std::make_unique<Memory>(0x4000)), cpu(std::make_unique<CPU>()), disp(std::make_unique<Display>()) {
	ppu = std::make_unique<PPU>(ppu_mem.get());
}

int main() {
	std::unique_ptr<State> ctx;
	ctx->disp->init();
	ctx->cpu->build_ins_arr();
	while (ctx->disp->wait()) {}
	ctx->disp->kill();
	return 0;
}

#include <iostream>
#include "nesem.h"
#include "cpu.h"
#include "mem.h"
#include "ppu.h"
#include "disp.h"
#include <bitset>
int main() {
	std::unique_ptr<State> ctx = std::make_unique<State>();
	std::unique_ptr<Display> disp = std::make_unique<Display>();
	std::unique_ptr<CPU> cpu = std::make_unique<CPU>();

	ctx->mem[0x5] = 0x10;
	ctx->mem[0x6] = 0x15;
	ctx->mem[0x1510] = 0x15;
	ctx->mem[0x1512] = 0x42;
	ctx->a = 3;
	ctx->pc = 0x2000;
	ctx->p = 0xFF;
	ctx->x = 0x3;
	ctx->y = 0x4;
	ctx->mem[0x2000] = 0x19;
	ctx->mem[0x2001] = 0x02;
	
	disp->init(ctx.get(), 1);
	while (disp->wait()) { cpu->fde(ctx.get());}
	disp->kill();
	return 0;
}

#include <iostream>
#include "nesem.h"
#include "cpu.h"
#include "mem.h"
#include "ppu.h"
#include "disp.h"

int main() {
	std::unique_ptr<State> ctx = std::make_unique<State>();
	std::unique_ptr<Display> disp = std::make_unique<Display>();
	std::unique_ptr<CPU> cpu = std::make_unique<CPU>();
	disp->init(ctx.get(), 1);
	while (disp->wait()) {}
	disp->kill();
	return 0;
}

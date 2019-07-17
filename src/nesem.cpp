#include <iostream>
#include "display.h"
#include "nesem.h"
#include "cpu.h"

int main() {
	std::unique_ptr<Display> display;
	std::unique_ptr<Memory> mem;
	display->init();
	std::unique_ptr<CPU> cpu;
	cpu->build_ins_arr();
	while (display->wait()) {}
	display->kill();
	return 0;
}

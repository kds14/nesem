#include <iostream>
#include "display.h"
#include "nesem.h"
#include "cpu.h"

int main() {
	Display* display = new Display();
	display->init();
	CPU* cpu = new CPU();
	cpu->build_ins_arr();
	while (display->wait()) {}
	display->kill();
	return 0;
}

#include "cpu.h"

bool err_ins(state* ctx) {
	return false;
}

bool brk(state* ctx) {
	return true;
}

void CPU::build_ins_arr() {
	ins[0x0] = brk;
}

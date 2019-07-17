#include "cpu.h"

void brk(state* ctx) {
	ctx->pc += 2;
	ctx->cycles += 7;
}

void orA_imm(state* ctx) {
	ctx->cycles += 2;
	ctx->a = ctx->pc + 1;
	ctx->pc += 2;
	ctx->n = ctx->a >> 7;
	ctx->z = ctx->a == 0;
}

void CPU::build_ins_arr() {
	ins[0x00] = brk;
	ins[0x09] = orA_imm;
}

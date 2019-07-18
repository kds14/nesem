#include "cpu.h"

void brk(State* ctx) {
	ctx->pc += 2;
	ctx->cycles += 7;
}

void orA_imm(State* ctx) {
	ctx->cycles += 2;
	ctx->a = ctx->pc + 1;
	ctx->pc += 2;
	ctx->n = ctx->a >> 7;
	ctx->z = ctx->a == 0;
}

void CPU::fde(State* ctx) {
	uint8_t op = ctx->get_op();
	switch (op) {
		case 0x00:
			brk(ctx);
		case 0x09:
			orA_imm(ctx);
	}
}

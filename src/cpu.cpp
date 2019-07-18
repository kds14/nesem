#include "cpu.h"

uint16_t get_ind(State* ctx, uint8_t v) {
	return ((uint16_t*)(&ctx->mem[v]))[0];
}

uint8_t get_op(State* ctx) {
	return ctx->mem[ctx->pc];
}

uint8_t get_b1(State* ctx) {
	return ctx->mem[ctx->pc + 1];
}

uint8_t get_b2(State* ctx) {
	return ctx->mem[ctx->pc + 2];
}

uint16_t get_16(State* ctx) {
	return ((uint16_t*)&ctx->mem[ctx->pc + 1])[0];
}

uint8_t get_indx(State* ctx) {
	return ctx->mem[((uint16_t*)(&ctx->mem[get_b1(ctx) + ctx->x]))[0]];
}

uint8_t get_indy(State* ctx) {
	return ctx->mem[get_ind(ctx, get_b1(ctx)) + ctx->y];
}

uint8_t get_zp(State* ctx) {
	return ctx->mem[get_b1(ctx)];
}

uint8_t get_zpx(State* ctx) {
	return ctx->mem[(get_b1(ctx) + ctx->x) % 0x100];
}

uint8_t get_abs(State* ctx) {
	return ctx->mem[get_16(ctx)];
}

uint8_t get_absx(State* ctx) {
	return ctx->mem[get_16(ctx) + ctx->x];
}

uint8_t get_absy(State* ctx) {
	return ctx->mem[get_16(ctx) + ctx->y];
}



void brk(State* ctx) {
	ctx->pc += 3;
	ctx->cycles += 7;
}

void orA(State* ctx, uint8_t operand) {
	ctx->a |= operand;
	ctx->n = ctx->a >> 7;
	ctx->z = ctx->a == 0;
}

void orA_imm(State* ctx) {
	orA(ctx, get_b1(ctx));
	ctx->pc += 2;
	ctx->cycles += 2;
}

void orA_zp(State* ctx) {
	orA(ctx, get_zp(ctx));
	ctx->pc += 2;
	ctx->cycles += 3;
}

void orA_zpx(State* ctx) {
	orA(ctx, get_zpx(ctx));
	ctx->pc += 2;
	ctx->cycles += 4;
}

void orA_abs(State* ctx) {
	orA(ctx, get_abs(ctx));
	ctx->pc += 3;
	ctx->cycles += 4;
}

void orA_absx(State* ctx) {
	orA(ctx, get_absx(ctx));
	ctx->pc += 3;
	ctx->cycles += 4;
}

void orA_absy(State* ctx) {
	orA(ctx, get_absy(ctx));
	ctx->pc += 3;
	ctx->cycles += 4;
}

void orA_indy(State* ctx) {
	orA(ctx, get_indy(ctx));
	ctx->pc += 2;
	ctx->cycles += 5;
}

#include <bitset>
void orA_indx(State* ctx) {
	orA(ctx, get_indx(ctx));
	ctx->pc += 2;
	ctx->cycles += 6;
}

void CPU::fde(State* ctx) {
	uint8_t op = get_op(ctx);
	std::cout << "A: " << (int)ctx->a << std::endl;	
	std::cout << "P: " <<  std::bitset<8>(ctx->p) << std::endl;
	std::cout << "B1: " << (int)get_b1(ctx) << std::endl;	
	std::cout << "X: " << (int)ctx->x << std::endl;	
	std::cout << "Y: " << (int)ctx->y << std::endl;	
	switch (op) {
		case 0x00:
			brk(ctx);
			break;
		case 0x01:
			orA_indx(ctx);
			break;
		case 0x05:
			orA_zp(ctx);
			break;
		case 0x09:
			orA_imm(ctx);
			break;
		case 0x0D:
			orA_abs(ctx);
			break;
		case 0x11:
			orA_indy(ctx);
			break;
		case 0x15:
			orA_zpx(ctx);
			break;
		case 0x19:
			orA_absy(ctx);
			break;
		case 0x1D:
			orA_absx(ctx);
			break;
	}
	std::cout << "A: " << std::hex << (int)ctx->a << std::endl;	
	std::cout << "P: " <<  std::bitset<8>(ctx->p) << std::endl;
	exit(0);
}

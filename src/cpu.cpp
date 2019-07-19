#include "cpu.h"

/*
 * Addressing mode helper functions
 */

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

uint8_t* get_indx_ptr(State* ctx) {
	return &ctx->mem[((uint16_t*)(&ctx->mem[get_b1(ctx) + ctx->x]))[0]];
}

uint8_t* get_indy_ptr(State* ctx) {
	return &ctx->mem[get_ind(ctx, get_b1(ctx)) + ctx->y];
}

uint8_t get_zp(State* ctx) {
	return ctx->mem[get_b1(ctx)];
}

uint8_t* get_zp_ptr(State* ctx) {
	return &ctx->mem[get_b1(ctx)];
}

uint8_t get_zpx(State* ctx) {
	return ctx->mem[(get_b1(ctx) + ctx->x) % 0x100];
}

uint8_t* get_zpx_ptr(State* ctx) {
	return &ctx->mem[(get_b1(ctx) + ctx->x) % 0x100];
}

uint8_t get_abs(State* ctx) {
	return ctx->mem[get_16(ctx)];
}

uint8_t* get_abs_ptr(State* ctx) {
	return &ctx->mem[get_16(ctx)];
}

uint8_t get_absx(State* ctx) {
	return ctx->mem[get_16(ctx) + ctx->x];
}

uint8_t* get_absx_ptr(State* ctx) {
	return &ctx->mem[get_16(ctx) + ctx->x];
}

uint8_t get_absy(State* ctx) {
	return ctx->mem[get_16(ctx) + ctx->y];
}

uint8_t* get_absy_ptr(State* ctx) {
	return &ctx->mem[get_16(ctx) + ctx->y];
}

/*
 * CPU instructions
 */

void brk(State* ctx) {
	ctx->pc += 3;
	ctx->cycles += 7;
}

void LDA(State* ctx, uint8_t operand) {
	ctx->a = operand;
	ctx->n = ctx->a >> 7;
	ctx->z = ctx->a == 0;
}

void LDA_imm(State* ctx) {
	LDA(ctx, get_b1(ctx));
	ctx->pc += 2;
	ctx->cycles += 2;
}

void LDA_zp(State* ctx) {
	LDA(ctx, get_zp(ctx));
	ctx->pc += 2;
	ctx->cycles += 3;
}

void LDA_zpx(State* ctx) {
	LDA(ctx, get_zpx(ctx));
	ctx->pc += 2;
	ctx->cycles += 4;
}

void LDA_abs(State* ctx) {
	LDA(ctx, get_abs(ctx));
	ctx->pc += 3;
	ctx->cycles += 4;
}

void LDA_absx(State* ctx) {
	LDA(ctx, get_absx(ctx));
	ctx->pc += 3;
	ctx->cycles += 4;
}

void LDA_absy(State* ctx) {
	LDA(ctx, get_absy(ctx));
	ctx->pc += 3;
	ctx->cycles += 4;
}

void LDA_indy(State* ctx) {
	LDA(ctx, get_indy(ctx));
	ctx->pc += 2;
	ctx->cycles += 5;
}

void LDA_indx(State* ctx) {
	LDA(ctx, get_indx(ctx));
	ctx->pc += 2;
	ctx->cycles += 6;
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

void orA_indx(State* ctx) {
	orA(ctx, get_indx(ctx));
	ctx->pc += 2;
	ctx->cycles += 6;
}

void EOR(State* ctx, uint8_t operand) {
	ctx->a ^= operand;
	ctx->n = ctx->a >> 7;
	ctx->z = ctx->a == 0;
}

void EOR_imm(State* ctx) {
	EOR(ctx, get_b1(ctx));
	ctx->pc += 2;
	ctx->cycles += 2;
}

void EOR_zp(State* ctx) {
	EOR(ctx, get_zp(ctx));
	ctx->pc += 2;
	ctx->cycles += 3;
}

void EOR_zpx(State* ctx) {
	EOR(ctx, get_zpx(ctx));
	ctx->pc += 2;
	ctx->cycles += 4;
}

void EOR_abs(State* ctx) {
	EOR(ctx, get_abs(ctx));
	ctx->pc += 3;
	ctx->cycles += 4;
}

void EOR_absx(State* ctx) {
	EOR(ctx, get_absx(ctx));
	ctx->pc += 3;
	ctx->cycles += 4;
}

void EOR_absy(State* ctx) {
	EOR(ctx, get_absy(ctx));
	ctx->pc += 3;
	ctx->cycles += 4;
}

void EOR_indy(State* ctx) {
	EOR(ctx, get_indy(ctx));
	ctx->pc += 2;
	ctx->cycles += 5;
}

void EOR_indx(State* ctx) {
	EOR(ctx, get_indx(ctx));
	ctx->pc += 2;
	ctx->cycles += 6;
}

void STA(State* ctx, uint8_t* ptr) {
	*ptr = ctx->a;
}

void STA_zp(State* ctx) {
	STA(ctx, get_zp_ptr(ctx));
	ctx->pc += 2;
	ctx->cycles += 3;
}

void STA_zpx(State* ctx) {
	STA(ctx, get_zpx_ptr(ctx));
	ctx->pc += 2;
	ctx->cycles += 4;
}

void STA_abs(State* ctx) {
	STA(ctx, get_abs_ptr(ctx));
	ctx->pc += 3;
	ctx->cycles += 4;
}

void STA_absx(State* ctx) {
	STA(ctx, get_absx_ptr(ctx));
	ctx->pc += 3;
	ctx->cycles += 5;
}

void STA_absy(State* ctx) {
	STA(ctx, get_absy_ptr(ctx));
	ctx->pc += 3;
	ctx->cycles += 5;
}

void STA_indy(State* ctx) {
	STA(ctx, get_indy_ptr(ctx));
	ctx->pc += 2;
	ctx->cycles += 6;
}

void STA_indx(State* ctx) {
	STA(ctx, get_indx_ptr(ctx));
	ctx->pc += 2;
	ctx->cycles += 6;
}

void ASL(State* ctx, uint8_t* ptr) {
	uint8_t bit7 = *ptr >> 7;
	uint8_t res = *ptr << 1;
	ctx->c = bit7;
	ctx->n = res >> 7;
	ctx->z = res == 0;
	*ptr = res;
}

void ASL_acc(State* ctx) {
	ASL(ctx, &ctx->a);
	ctx->pc += 1;
	ctx->cycles += 2;
}

void ASL_zp(State* ctx) {
	ASL(ctx, get_zp_ptr(ctx));
	ctx->pc += 2;
	ctx->cycles += 5;
}

void ASL_zpx(State* ctx) {
	ASL(ctx, get_zpx_ptr(ctx));
	ctx->pc += 2;
	ctx->cycles += 6;
}

void ASL_abs(State* ctx) {
	ASL(ctx, get_abs_ptr(ctx));
	ctx->pc += 3;
	ctx->cycles += 6;
}

void ASL_absx(State* ctx) {
	ASL(ctx, get_absx_ptr(ctx));
	ctx->pc += 3;
	ctx->cycles += 7;
}

void AND(State* ctx, uint8_t operand) {
	ctx->a &= operand;
	ctx->n = ctx->a >> 7;
	ctx->z = ctx->a == 0;
}

void AND_imm(State* ctx) {
	AND(ctx, get_b1(ctx));
	ctx->pc += 2;
	ctx->cycles += 2;
}

void AND_zp(State* ctx) {
	AND(ctx, get_zp(ctx));
	ctx->pc += 2;
	ctx->cycles += 3;
}

void AND_zpx(State* ctx) {
	AND(ctx, get_zpx(ctx));
	ctx->pc += 2;
	ctx->cycles += 4;
}

void AND_abs(State* ctx) {
	AND(ctx, get_abs(ctx));
	ctx->pc += 3;
	ctx->cycles += 4;
}

void AND_absx(State* ctx) {
	AND(ctx, get_absx(ctx));
	ctx->pc += 3;
	ctx->cycles += 4;
}

void AND_absy(State* ctx) {
	AND(ctx, get_absy(ctx));
	ctx->pc += 3;
	ctx->cycles += 4;
}

void AND_indy(State* ctx) {
	AND(ctx, get_indy(ctx));
	ctx->pc += 2;
	ctx->cycles += 5;
}

void AND_indx(State* ctx) {
	AND(ctx, get_indx(ctx));
	ctx->pc += 2;
	ctx->cycles += 6;
}

	#include <bitset>
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
		case 0x06:
			ASL_zp(ctx);
			break;
		case 0x09:
			orA_imm(ctx);
			break;
		case 0x0A:
			ASL_acc(ctx);
			break;
		case 0x0D:
			orA_abs(ctx);
			break;
		case 0x0E:
			ASL_abs(ctx);
			break;
		case 0x11:
			orA_indy(ctx);
			break;
		case 0x15:
			orA_zpx(ctx);
			break;
		case 0x16:
			ASL_zpx(ctx);
			break;
		case 0x19:
			orA_absy(ctx);
			break;
		case 0x1D:
			orA_absx(ctx);
			break;
		case 0x1E:
			ASL_absx(ctx);
			break;
		case 0x21:
			AND_indx(ctx);
			break;
		case 0x25:
			AND_zp(ctx);
			break;
		case 0x29:
			AND_imm(ctx);
			break;
		case 0x2D:
			AND_abs(ctx);
			break;
		case 0x31:
			AND_indy(ctx);
			break;
		case 0x35:
			AND_zpx(ctx);
			break;
		case 0x39:
			AND_absy(ctx);
			break;
		case 0x3D:
			AND_absx(ctx);
			break;
		case 0x41:
			EOR_indx(ctx);
			break;
		case 0x45:
			EOR_zp(ctx);
			break;
		case 0x49:
			EOR_imm(ctx);
			break;
		case 0x4D:
			EOR_abs(ctx);
			break;
		case 0x51:
			EOR_indy(ctx);
			break;
		case 0x55:
			EOR_zpx(ctx);
			break;
		case 0x59:
			EOR_absy(ctx);
			break;
		case 0x5D:
			EOR_absx(ctx);
			break;
		case 0x81:
			STA_indx(ctx);
			break;
		case 0x85:
			STA_zp(ctx);
			break;
		case 0x8D:
			STA_abs(ctx);
			break;
		case 0x91:
			STA_indy(ctx);
			break;
		case 0x95:
			STA_zpx(ctx);
			break;
		case 0x99:
			STA_absy(ctx);
			break;
		case 0x9D:
			STA_absx(ctx);
			break;
		case 0xA1:
			LDA_indx(ctx);
			break;
		case 0xA5:
			LDA_zp(ctx);
			break;
		case 0xA9:
			LDA_imm(ctx);
			break;
		case 0xAD:
			LDA_abs(ctx);
			break;
		case 0xB1:
			LDA_indy(ctx);
			break;
		case 0xB5:
			LDA_zpx(ctx);
			break;
		case 0xB9:
			LDA_absy(ctx);
			break;
		case 0xBD:
			LDA_absx(ctx);
			break;
	}
	std::cout << "A: " << std::hex << (int)ctx->a << std::endl;	
	std::cout << "P: " <<  std::bitset<8>(ctx->p) << std::endl;
	exit(0);
}

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

uint8_t get_zpy(State* ctx) {
	return ctx->mem[(get_b1(ctx) + ctx->y) % 0x100];
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

void JMP_abs(State* ctx) {
	ctx->pc = get_16(ctx);
	ctx->cycles = 3;
}

void JMP_ind(State* ctx) {
	// TODO: bug when using last byte of page
	ctx->pc = ((uint16_t*)(&ctx->mem[get_16(ctx)]))[0];
	ctx->cycles = 5;
}

void brk(State* ctx) {
	ctx->pc += 3;
	ctx->cycles += 7;
}

void LD_reg(State* ctx, uint8_t* reg, uint8_t operand) {
	*reg = operand;
	ctx->n = operand >> 7;
	ctx->z = operand == 0;
}

void LDY_imm(State* ctx) {
	LD_reg(ctx, &ctx->y, get_b1(ctx));
	ctx->pc += 2;
	ctx->cycles += 2;
}

void LDY_zp(State* ctx) {
	LD_reg(ctx, &ctx->y, get_zp(ctx));
	ctx->pc += 2;
	ctx->cycles += 3;
}

void LDY_zpx(State* ctx) {
	LD_reg(ctx, &ctx->y, get_zpx(ctx));
	ctx->pc += 2;
	ctx->cycles += 4;
}

void LDY_abs(State* ctx) {
	LD_reg(ctx, &ctx->y, get_abs(ctx));
	ctx->pc += 3;
	ctx->cycles += 4;
}

void LDY_absx(State* ctx) {
	LD_reg(ctx, &ctx->y, get_absx(ctx));
	ctx->pc += 3;
	ctx->cycles += 4;
}

void LDX_imm(State* ctx) {
	LD_reg(ctx, &ctx->x, get_b1(ctx));
	ctx->pc += 2;
	ctx->cycles += 2;
}

void LDX_zp(State* ctx) {
	LD_reg(ctx, &ctx->x, get_zp(ctx));
	ctx->pc += 2;
	ctx->cycles += 3;
}

void LDX_zpy(State* ctx) {
	LD_reg(ctx, &ctx->x, get_zpy(ctx));
	ctx->pc += 2;
	ctx->cycles += 4;
}

void LDX_abs(State* ctx) {
	LD_reg(ctx, &ctx->x, get_abs(ctx));
	ctx->pc += 3;
	ctx->cycles += 4;
}

void LDX_absy(State* ctx) {
	LD_reg(ctx, &ctx->x, get_absy(ctx));
	ctx->pc += 3;
	ctx->cycles += 4;
}

void LDA_imm(State* ctx) {
	LD_reg(ctx, &ctx->a, get_b1(ctx));
	ctx->pc += 2;
	ctx->cycles += 2;
}

void LDA_zp(State* ctx) {
	LD_reg(ctx, &ctx->a, get_zp(ctx));
	ctx->pc += 2;
	ctx->cycles += 3;
}

void LDA_zpx(State* ctx) {
	LD_reg(ctx, &ctx->a, get_zpx(ctx));
	ctx->pc += 2;
	ctx->cycles += 4;
}

void LDA_abs(State* ctx) {
	LD_reg(ctx, &ctx->a, get_abs(ctx));
	ctx->pc += 3;
	ctx->cycles += 4;
}

void LDA_absx(State* ctx) {
	LD_reg(ctx, &ctx->a, get_absx(ctx));
	ctx->pc += 3;
	ctx->cycles += 4;
}

void LDA_absy(State* ctx) {
	LD_reg(ctx, &ctx->a, get_absy(ctx));
	ctx->pc += 3;
	ctx->cycles += 4;
}

void LDA_indy(State* ctx) {
	LD_reg(ctx, &ctx->a, get_indy(ctx));
	ctx->pc += 2;
	ctx->cycles += 5;
}

void LDA_indx(State* ctx) {
	LD_reg(ctx, &ctx->a, get_indx(ctx));
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

void DEC_INC(State* ctx, uint8_t* ptr, bool inc) {
	uint8_t val = inc ? ++(*ptr) : --(*ptr);
	ctx->n = (int8_t)val < 0;
	ctx->z = val == 0;
}

void DEC_zp(State* ctx) {
	DEC_INC(ctx, get_zp_ptr(ctx), false);
	ctx->pc += 2;
	ctx->cycles += 5;
}

void DEC_zpx(State* ctx) {
	DEC_INC(ctx, get_zpx_ptr(ctx), false);
	ctx->pc += 2;
	ctx->cycles += 6;
}

void DEC_abs(State* ctx) {
	DEC_INC(ctx, get_abs_ptr(ctx), false);
	ctx->pc += 3;
	ctx->cycles += 6;
}

void DEC_absx(State* ctx) {
	DEC_INC(ctx, get_absx_ptr(ctx), false);
	ctx->pc += 3;
	ctx->cycles += 7;
}

void INC_zp(State* ctx) {
	DEC_INC(ctx, get_zp_ptr(ctx), true);
	ctx->pc += 2;
	ctx->cycles += 5;
}

void INC_zpx(State* ctx) {
	DEC_INC(ctx, get_zpx_ptr(ctx), true);
	ctx->pc += 2;
	ctx->cycles += 6;
}

void INC_abs(State* ctx) {
	DEC_INC(ctx, get_abs_ptr(ctx), true);
	ctx->pc += 3;
	ctx->cycles += 6;
}

void INC_absx(State* ctx) {
	DEC_INC(ctx, get_absx_ptr(ctx), true);
	ctx->pc += 3;
	ctx->cycles += 7;
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

void LSR(State* ctx, uint8_t* ptr) {
	uint8_t bit0 = *ptr & 0x1;
	uint8_t res = *ptr >> 1;
	ctx->c = bit0;
	ctx->n = res >> 7;
	ctx->z = res == 0;
	*ptr = res;
}

void LSR_acc(State* ctx) {
	LSR(ctx, &ctx->a);
	ctx->pc += 1;
	ctx->cycles += 2;
}

void LSR_zp(State* ctx) {
	LSR(ctx, get_zp_ptr(ctx));
	ctx->pc += 2;
	ctx->cycles += 5;
}

void LSR_zpx(State* ctx) {
	LSR(ctx, get_zpx_ptr(ctx));
	ctx->pc += 2;
	ctx->cycles += 6;
}

void LSR_abs(State* ctx) {
	LSR(ctx, get_abs_ptr(ctx));
	ctx->pc += 3;
	ctx->cycles += 6;
}

void LSR_absx(State* ctx) {
	LSR(ctx, get_absx_ptr(ctx));
	ctx->pc += 3;
	ctx->cycles += 7;
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

void CMP(State* ctx, uint8_t operand, uint8_t reg) {
	uint16_t val = ctx->c + operand + ctx->a;
	ctx->c = reg >= operand;
	ctx->n = operand > reg;
	ctx->z = reg == operand;
}

void CMP_imm(State* ctx, uint8_t reg) {
	CMP(ctx, get_b1(ctx), reg);
	ctx->pc += 2;
	ctx->cycles += 2;
}

void CMP_zp(State* ctx, uint8_t reg) {
	CMP(ctx, get_zp(ctx), reg);
	ctx->pc += 2;
	ctx->cycles += 3;
}

void CMP_zpx(State* ctx) {
	CMP(ctx, get_zpx(ctx), ctx->a);
	ctx->pc += 2;
	ctx->cycles += 4;
}

void CMP_abs(State* ctx, uint8_t reg) {
	CMP(ctx, get_abs(ctx), reg);
	ctx->pc += 3;
	ctx->cycles += 4;
}

void CMP_absx(State* ctx) {
	CMP(ctx, get_absx(ctx), ctx->a);
	ctx->pc += 3;
	ctx->cycles += 4;
}

void CMP_absy(State* ctx) {
	CMP(ctx, get_absy(ctx), ctx->a);
	ctx->pc += 3;
	ctx->cycles += 4;
}

void CMP_indy(State* ctx) {
	CMP(ctx, get_indy(ctx), ctx->a);
	ctx->pc += 2;
	ctx->cycles += 5;
}

void CMP_indx(State* ctx) {
	CMP(ctx, get_indx(ctx), ctx->a);
	ctx->pc += 2;
	ctx->cycles += 6;
}

void ADC(State* ctx, uint8_t operand) {
	uint16_t val = ctx->c + operand + ctx->a;
	ctx->v = (ctx->a ^ val) & (operand ^ val) & 0x80;
	ctx->c = val >> 8;
	ctx->a = val;
	ctx->n = ctx->a >> 7;
	ctx->z = ctx->a == 0;
}

void ADC_imm(State* ctx) {
	ADC(ctx, get_b1(ctx));
	ctx->pc += 2;
	ctx->cycles += 2;
}

void ADC_zp(State* ctx) {
	ADC(ctx, get_zp(ctx));
	ctx->pc += 2;
	ctx->cycles += 3;
}

void ADC_zpx(State* ctx) {
	ADC(ctx, get_zpx(ctx));
	ctx->pc += 2;
	ctx->cycles += 4;
}

void ADC_abs(State* ctx) {
	ADC(ctx, get_abs(ctx));
	ctx->pc += 3;
	ctx->cycles += 4;
}

void ADC_absx(State* ctx) {
	ADC(ctx, get_absx(ctx));
	ctx->pc += 3;
	ctx->cycles += 4;
}

void ADC_absy(State* ctx) {
	ADC(ctx, get_absy(ctx));
	ctx->pc += 3;
	ctx->cycles += 4;
}

void ADC_indy(State* ctx) {
	ADC(ctx, get_indy(ctx));
	ctx->pc += 2;
	ctx->cycles += 5;
}

void ADC_indx(State* ctx) {
	ADC(ctx, get_indx(ctx));
	ctx->pc += 2;
	ctx->cycles += 6;
}

void BIT(State* ctx, uint8_t operand) {
	ctx->z = ctx->a & operand;
	ctx->n = operand >> 7;
	ctx->v = operand >> 6;
}

void BIT_zp(State* ctx) {
	BIT(ctx, get_zp(ctx));
	ctx->pc += 2;
	ctx->cycles += 3;
}

void BIT_abs(State* ctx) {
	BIT(ctx, get_abs(ctx));
	ctx->pc += 3;
	ctx->cycles += 4;
}

void branch(State* ctx, uint8_t condition) {
	ctx->pc += 2;
	ctx->cycles += 2;
	if (condition) {
		ctx->pc += (int8_t)get_op(ctx);
		ctx->cycles += 1;
	}
}

void set_p(State* ctx, uint8_t bit, bool set) {
	ctx->p = set ? ctx->p | bit : ctx->p & ~bit;
	ctx->pc += 1;
	ctx->cycles += 2;
}

void push_stack(State* ctx, uint8_t val) {
	ctx->mem[--ctx->s] = val;
}

uint8_t pop_stack(State* ctx, uint8_t val) {
	return ctx->mem[ctx->s++];
}

void JSR(State* ctx) {
	ctx->pc += 3;
	ctx->cycles += 6;
	push_stack(ctx, ctx->pc);
	push_stack(ctx, ctx->pc - 1);
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
		case 0x10:
			branch(ctx, !ctx->n);
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
		case 0x18:
			// CLC
			set_p(ctx, 0x1, false);
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
		case 0x20:
			JSR(ctx);
			break;
		case 0x21:
			AND_indx(ctx);
			break;
		case 0x24:
			BIT_zp(ctx);
			break;
		case 0x25:
			AND_zp(ctx);
			break;
		case 0x29:
			AND_imm(ctx);
			break;
		case 0x2C:
			BIT_abs(ctx);
			break;
		case 0x2D:
			AND_abs(ctx);
			break;
		case 0x30:
			branch(ctx, ctx->n);
			break;
		case 0x31:
			AND_indy(ctx);
			break;
		case 0x35:
			AND_zpx(ctx);
			break;
		case 0x38:
			// SEC
			set_p(ctx, 0x1, true);
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
		case 0x46:
			LSR_zp(ctx);
			break;
		case 0x49:
			EOR_imm(ctx);
			break;
		case 0x4A:
			LSR_acc(ctx);
			break;
		case 0x4C:
			JMP_abs(ctx);
			break;
		case 0x4D:
			EOR_abs(ctx);
			break;
		case 0x4E:
			LSR_abs(ctx);
			break;
		case 0x50:
			branch(ctx, !ctx->v);
			break;
		case 0x51:
			EOR_indy(ctx);
			break;
		case 0x55:
			EOR_zpx(ctx);
			break;
		case 0x56:
			LSR_zpx(ctx);
			break;
		case 0x58:
			// CLI
			set_p(ctx, 0x4, false);
			break;
		case 0x59:
			EOR_absy(ctx);
			break;
		case 0x5D:
			EOR_absx(ctx);
			break;
		case 0x5E:
			LSR_absx(ctx);
			break;
		case 0x61:
			ADC_indx(ctx);
			break;
		case 0x65:
			ADC_zp(ctx);
			break;
		case 0x69:
			ADC_imm(ctx);
			break;
		case 0x6C:
			JMP_ind(ctx);
			break;
		case 0x6D:
			ADC_abs(ctx);
			break;
		case 0x70:
			branch(ctx, ctx->v);
			break;
		case 0x71:
			ADC_indy(ctx);
			break;
		case 0x75:
			ADC_zpx(ctx);
			break;
		case 0x78:
			// SEI
			set_p(ctx, 0x4, true);
			break;
		case 0x79:
			ADC_absy(ctx);
			break;
		case 0x7D:
			ADC_absx(ctx);
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
		case 0x90:
			branch(ctx, !ctx->c);
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
		case 0xA0:
			LDY_imm(ctx);
			break;
		case 0xA1:
			LDA_indx(ctx);
			break;
		case 0xA2:
			LDX_imm(ctx);
			break;
		case 0xA4:
			LDY_zp(ctx);
			break;
		case 0xA5:
			LDA_zp(ctx);
			break;
		case 0xA6:
			LDX_zp(ctx);
			break;
		case 0xA9:
			LDA_imm(ctx);
			break;
		case 0xAC:
			LDY_abs(ctx);
			break;
		case 0xAD:
			LDA_abs(ctx);
			break;
		case 0xAE:
			LDX_abs(ctx);
			break;
		case 0xB0:
			branch(ctx, ctx->c);
			break;
		case 0xB1:
			LDA_indy(ctx);
			break;
		case 0xB4:
			LDY_zpx(ctx);
			break;
		case 0xB5:
			LDA_zpx(ctx);
			break;
		case 0xB6:
			LDX_zpy(ctx);
			break;
		case 0xB8:
			// CLV
			set_p(ctx, 0x40, false);
			break;
		case 0xB9:
			LDA_absy(ctx);
			break;
		case 0xBC:
			LDY_absx(ctx);
			break;
		case 0xBD:
			LDA_absx(ctx);
			break;
		case 0xBE:
			LDX_absy(ctx);
			break;
		case 0xC0:
			CMP_imm(ctx, ctx->y);
			break;
		case 0xC1:
			CMP_indx(ctx);
			break;
		case 0xC4:
			CMP_zp(ctx, ctx->y);
			break;
		case 0xC5:
			CMP_zp(ctx, ctx->a);
			break;
		case 0xC6:
			DEC_zp(ctx);
			break;
		case 0xC9:
			CMP_imm(ctx, ctx->a);
			break;
		case 0xCC:
			CMP_abs(ctx, ctx->y);
			break;
		case 0xCD:
			CMP_abs(ctx, ctx->a);
			break;
		case 0xCE:
			DEC_abs(ctx);
			break;
		case 0xD0:
			branch(ctx, !ctx->z);
			break;
		case 0xD1:
			CMP_indy(ctx);
			break;
		case 0xD5:
			CMP_zpx(ctx);
			break;
		case 0xD6:
			DEC_zpx(ctx);
			break;
		case 0xD8:
			// CLD
			set_p(ctx, 0x8, false);
			break;
		case 0xD9:
			CMP_absy(ctx);
			break;
		case 0xDD:
			CMP_absx(ctx);
			break;
		case 0xDE:
			DEC_absx(ctx);
			break;
		case 0xE0:
			CMP_imm(ctx, ctx->x);
			break;
		case 0xE4:
			CMP_zp(ctx, ctx->x);
			break;
		case 0xE6:
			INC_zp(ctx);
			break;
		case 0xEC:
			CMP_abs(ctx, ctx->x);
			break;
		case 0xEE:
			INC_abs(ctx);
			break;
		case 0xF0:
			branch(ctx, ctx->z);
			break;
		case 0xF6:
			INC_zpx(ctx);
			break;
		case 0xF8:
			// SED
			set_p(ctx, 0x8, true);
			break;
		case 0xFE:
			INC_absx(ctx);
			break;
	}
	std::cout << "A: " << std::hex << (int)ctx->a << std::endl;	
	std::cout << "P: " <<  std::bitset<8>(ctx->p) << std::endl;
	exit(0);
}

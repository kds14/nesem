#ifndef CPU_H
#define CPU_H

#include "nesem.h"

typedef bool (*cpu_ins)(state*);

bool err_ins(state* ctx);

class CPU {
private:
	std::vector<cpu_ins> ins;
public:
	void set_ins();
	CPU() : ins(std::vector<cpu_ins>(0x100, err_ins)) {}
	void build_ins_arr();
};

#endif

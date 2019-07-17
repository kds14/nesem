#ifndef CPU_H
#define CPU_H

#include "nesem.h"

typedef void (*cpu_ins)(state*);

class CPU {
private:
	std::vector<cpu_ins> ins;
public:
	void set_ins();
	CPU() : ins(std::vector<cpu_ins>(0x100, nullptr)) {}
	void build_ins_arr();
};

#endif

#ifndef CPU_H
#define CPU_H

#include "nesem.h"

struct State;

class CPU {
public:
	void fde(State* ctx);
};

#endif

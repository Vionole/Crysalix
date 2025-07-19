#pragma once

#include "Instruction.h"

class Machine {
public:
	bool debug;
	unsigned int tmp_count;
	unsigned int instruct_number;

	map<wstring, Var> in_data;
	Var ret_data;
	vector<Instruction> instructions;
	unsigned int instruct_count = 0;

	map<wstring, Var> heap;
	map<wstring, int> jmp_pointers;

	map<wstring, Machine> sub_machines;
	
	Machine(map<wstring, Var> in, bool dbg = false);

	void prepare();

	Var go();

};
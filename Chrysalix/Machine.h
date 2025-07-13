#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "VarClass.h"


using namespace std;

class Machine;

enum OP_CODE {
	NOP,
	END,
	PAUSE,
	SLEEP,
	VAR,
	PRINT,
	FREE,
	LABEL,
	JUMP,
	INPT,
	CHANGE,
	TO,
	CALC,
	NEWTEMP,
	FORGET,
	TCOUNT,
	ISSET,
	TYPEOF,
	COMP,
	LOGIC,
	JIF,
	JIFNOT,
	DLABEL,
	SWAP,
	ARRAY,
	VTOARR,
	PUSHB,
	POPB,
	PUSHF,
	POPF,
	ERASE,
	INSRT,
	CLEAR,
	SIZEARR,
	GETVAL,
	SETVAL
};

struct Instruction {
	OP_CODE opCode = NOP;
	vector<Var> parameters;
	wstring as_string;
};

typedef void (*func)(Machine* m, Instruction* i, bool prevalidate, bool prego);

class Machine {
public:
	bool debug;
	int tmp_count;
	int instruct_number;

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
#pragma once

#include "CTable.h"
#include "Var.h"

struct Instruction {
	OP_CODE opCode = NOP;
	vector<Var> parameters;
	wstring as_string;
};

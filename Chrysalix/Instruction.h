#pragma once
#include <string>
#include <vector>
#include <map>

#include "CTables.h"
#include "Var.h"

using namespace std;

struct Instruction {
	OP_CODE opCode = NOP;
	vector<Var> parameters;
	wstring as_string;
};

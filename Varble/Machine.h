#pragma once
#include <string>
#include <vector>
#include <map>
#include "VarClass.h"

using namespace std;

class Machine;

class Instruct {
private:
	vector<Var> values;
public:
	virtual void go(Machine m) = 0;
	virtual ~Instruct() {};
};

class Machine {
public:
	int instruct_number;

	vector<Var> in_data;
	Var ret_data;

	vector<Instruct*> instructions;

	map<wstring, Var> heap;
	map<wstring, int> jmp_pointers;

	map<wstring, Machine> sub_machines;
	
public:
	Machine(vector<Var> in);

	void prepare();

	void go();

};
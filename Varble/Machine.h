#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "VarClass.h"

using namespace std;

class Machine;

class Instruct {
public:
	vector<Var> values;
	wstring name;

	virtual void go(Machine& m) = 0;
	virtual bool validate(Machine& m) = 0;
	virtual ~Instruct() {};
};

class InstructNOP: public Instruct {
public:
	InstructNOP(vector<Var> val);
	void go(Machine& m) override;
	bool validate(Machine& m) override;
};

class InstructEND : public Instruct {
public:
	InstructEND(vector<Var> val);
	void go(Machine& m) override;
	bool validate(Machine& m) override;
};

class InstructVAR : public Instruct {
public:
	InstructVAR(vector<Var> val);
	void go(Machine& m) override;
	bool validate(Machine& m) override;
};

class InstructPRINT : public Instruct {
public:
	InstructPRINT(vector<Var> val);
	void go(Machine& m) override;
	bool validate(Machine& m) override;
};

class Machine {
public:
	int instruct_number;

	map<wstring, Var> in_data;
	Var ret_data;

	vector<Instruct*> instructions;

	map<wstring, Var> heap;
	map<wstring, int> jmp_pointers;

	map<wstring, Machine> sub_machines;
	
	Machine(map<wstring, Var> in);

	void prepare();

	Var go();

};
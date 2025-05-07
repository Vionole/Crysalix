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

class InstructPAUSE : public Instruct {
public:
	InstructPAUSE(vector<Var> val);
	void go(Machine& m) override;
	bool validate(Machine& m) override;
};

class InstructSLEEP : public Instruct {
public:
	InstructSLEEP(vector<Var> val);
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

class InstructFREE : public Instruct {
public:
	InstructFREE(vector<Var> val);
	void go(Machine& m) override;
	bool validate(Machine& m) override;
};

class InstructLBL : public Instruct {
public:
	InstructLBL(vector<Var> val);
	void go(Machine& m) override;
	bool validate(Machine& m) override;
};

class InstructJMP : public Instruct {
public:
	InstructJMP(vector<Var> val);
	void go(Machine& m) override;
	bool validate(Machine& m) override;
};

class InstructJMPIFZ : public Instruct {
public:
	InstructJMPIFZ(vector<Var> val);
	void go(Machine& m) override;
	bool validate(Machine& m) override;
};

class InstructJMPIFNOTZ : public Instruct {
public:
	InstructJMPIFNOTZ(vector<Var> val);
	void go(Machine& m) override;
	bool validate(Machine& m) override;
};

class InstructINPUT : public Instruct {
public:
	InstructINPUT(vector<Var> val);
	void go(Machine& m) override;
	bool validate(Machine& m) override;
};

class InstructCHNG : public Instruct {
public:
	InstructCHNG(vector<Var> val);
	void go(Machine& m) override;
	bool validate(Machine& m) override;
};


class InstructTO : public Instruct {
public:
	InstructTO(vector<Var> val);
	void go(Machine& m) override;
	bool validate(Machine& m) override;
};


class Machine {
public:
	bool debug;
	int instruct_number;

	map<wstring, Var> in_data;
	Var ret_data;

	vector<Instruct*> instructions;

	map<wstring, Var> heap;
	map<wstring, int> jmp_pointers;

	map<wstring, Machine> sub_machines;
	
	Machine(map<wstring, Var> in, bool dbg = false);

	void prepare();

	Var go();

};
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

	virtual void go(Machine* m, bool prego) = 0;
	virtual bool validate(Machine* m, bool prevalidate) = 0;
	virtual ~Instruct() {};
};

class InstructNOP: public Instruct {
public:
	InstructNOP(vector<Var>* val);
	void go(Machine* m, bool prego) override;
	bool validate(Machine* m, bool prevalidate) override;
};

class InstructEND : public Instruct {
public:
	InstructEND(vector<Var>* val);
	void go(Machine* m, bool prego) override;
	bool validate(Machine* m, bool prevalidate) override;
};

class InstructPAUSE : public Instruct {
public:
	InstructPAUSE(vector<Var>* val);
	void go(Machine* m, bool prego) override;
	bool validate(Machine* m, bool prevalidate) override;
};

class InstructSLEEP : public Instruct {
public:
	InstructSLEEP(vector<Var>* val);
	void go(Machine* m, bool prego) override;
	bool validate(Machine* m, bool prevalidate) override;
};

class InstructVARIABLE: public Instruct {
public:
	InstructVARIABLE(vector<Var>* val);
	void go(Machine* m, bool prego) override;
	bool validate(Machine* m, bool prevalidate) override;
};

class InstructPRINT : public Instruct {
public:
	InstructPRINT(vector<Var>* val);
	void go(Machine* m, bool prego) override;
	bool validate(Machine* m, bool prevalidate) override;
};

class InstructFREE : public Instruct {
public:
	InstructFREE(vector<Var>* val);
	void go(Machine* m, bool prego) override;
	bool validate(Machine* m, bool prevalidate) override;
};

class InstructLABEL : public Instruct {
public:
	InstructLABEL(vector<Var>* val);
	void go(Machine* m, bool prego) override;
	bool validate(Machine* m, bool prevalidate) override;
};

class InstructJUMP : public Instruct {
public:
	InstructJUMP(vector<Var>* val);
	void go(Machine* m, bool prego) override;
	bool validate(Machine* m, bool prevalidate) override;
};

class InstructINPUT : public Instruct {
public:
	InstructINPUT(vector<Var>* val);
	void go(Machine* m, bool prego) override;
	bool validate(Machine* m, bool prevalidate) override;
};

class InstructCHANGE : public Instruct {
public:
	InstructCHANGE(vector<Var>* val);
	void go(Machine* m, bool prego) override;
	bool validate(Machine* m, bool prevalidate) override;
};


class InstructTO : public Instruct {
public:
	InstructTO(vector<Var>* val);
	void go(Machine* m, bool prego) override;
	bool validate(Machine* m, bool prevalidate) override;
};

class InstructCALC : public Instruct {
public:
	InstructCALC(vector<Var>* val);
	void go(Machine* m, bool prego) override;
	bool validate(Machine* m, bool prevalidate) override;
};


class Machine {
public:
	bool debug;
	int instruct_number;

	map<wstring, Var> in_data;
	Var ret_data;

	vector<Instruct*> instructions;
	unsigned int instruct_count = 0;

	map<wstring, Var> heap;
	map<wstring, int> jmp_pointers;

	map<wstring, Machine> sub_machines;
	
	Machine(map<wstring, Var> in, bool dbg = false);

	void prepare();

	Var go();

};
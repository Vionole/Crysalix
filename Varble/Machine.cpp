#include "Machine.h"

Machine::Machine(map<wstring, Var> in) {
	this->in_data = in;
}

void Machine::prepare() {
	this->instruct_number = 0;
}

Var Machine::go() {
	while (this->instruct_number != -1) {
		wcout << this->instruct_number << L": " << this->instructions[this->instruct_number]->name << endl;
		wcout << L"-------------------------------------------------------" << endl;
		wcout << L"Heap:" << endl;
		for (const auto& kv : this->heap) {
			wcout << kv.first << ": " << kv.second << endl;
		}
		wcout << L"=======================================================" << endl;

		if (this->instructions[this->instruct_number]->validate(*this)) {
			this->instructions[this->instruct_number]->go(*this);
		}
	}
	return this->ret_data;
}

InstructNOP::InstructNOP(vector<Var> val) {
	this->name = L"NOP";
	this->values = val;
}

void InstructNOP::go(Machine& m) {
	++m.instruct_number;
}

bool InstructNOP::validate(Machine& m) {
	if (this->values.size() == 0) {
		return true;
	}
	else {
		throw wstring{ to_wstring(m.instruct_number + 1) + L": Инструкция NOP принимает 0 параметров\n" };
	}
}


InstructEND::InstructEND(vector<Var> val) {
	this->name = L"END";
	this->values = val;
}

void InstructEND::go(Machine& m) {
	if (this->values[0].toSTR().slice(0, 1).getWStr() == L"$") {
		m.ret_data = m.heap[this->values[0].getWStr()];
	}
	else {
		m.ret_data = this->values[0];
	}
	m.instruct_number = -1;

}

bool InstructEND::validate(Machine& m) {
	if (this->values.size() == 1) {
		if (this->values[0].typeOf() == L"STR" && this->values[0].toSTR().slice(0, 1).getWStr() == L"$") {
			if (m.heap.find(this->values[0].getWStr()) == m.heap.end()) {
				throw wstring{ to_wstring(m.instruct_number + 1) + L": Переменная " + this->values[0].getWStr() + L" не существует\n" };
			}
			else {
				return true;
			}
		}
		else {
			return true;
		}
	}
	else {
		throw wstring{ to_wstring(m.instruct_number + 1) + L": Инструкция END принимает 1 параметр\n" };
	}
}


InstructVAR::InstructVAR(vector<Var> val) {
	this->name = L"VAR";
	this->values = val;
}

void InstructVAR::go(Machine& m) {
	if (this->values[1].toSTR().slice(0, 1).getWStr() == L"$") {
		m.heap[this->values[0].getWStr()] = m.heap[this->values[1].getWStr()];
	}
	else {
		m.heap[this->values[0].getWStr()] = this->values[1];
	}
	++m.instruct_number;
}

bool InstructVAR::validate(Machine& m) {
	if (this->values.size() == 2) {
		if (this->values[0].typeOf() == L"STR" && this->values[0].toSTR().slice(0, 1).getWStr() == L"$") {
			if (m.heap.find(this->values[0].getWStr()) == m.heap.end()) {
				return true;
			}
			else {
				throw wstring{ to_wstring(m.instruct_number + 1) + L": Переменная " + this->values[0].getWStr() + L" уже определена\n"};
			}
		}
		else {
			throw wstring{ to_wstring(m.instruct_number + 1) + L": Первый параметр инструкции VAR должен быть именем переменной\n" };
		}
	}
	else {
		throw wstring{ to_wstring(m.instruct_number + 1) + L": Инструкция VAR принимает 2 параметра\n" };
	}
}


InstructPRINT::InstructPRINT(vector<Var> val) {
	this->name = L"PRINT";
	this->values = val;
}

void InstructPRINT::go(Machine& m) {
	for (auto& i : this->values)
	{
		if (i.toSTR().slice(0, 1).getWStr() == L"$") {
			wcout << m.heap[i.getWStr()];
		}
		else {
			wcout << i;
		}
	}
	++m.instruct_number;
}

bool InstructPRINT::validate(Machine& m) {

	if (this->values.size() > 0) {
		for (auto& i : this->values)
		{
			if (i.toSTR().slice(0, 1).getWStr() == L"$") {
				if (m.heap.find(i.getWStr()) == m.heap.end()) {
					throw wstring{ to_wstring(m.instruct_number + 1) + L": Переменная " + i.getWStr() + L" не существует\n" };
				}
			}
		}
		return true;
	}
	else {
		throw wstring{ to_wstring(m.instruct_number + 1) + L": Инструкция PRINT принимает как минимум 1 параметр\n" };
	}
}

InstructFREE::InstructFREE(vector<Var> val) {
	this->name = L"FREE";
	this->values = val;
}

void InstructFREE::go(Machine& m) {
	for (auto& i : this->values)
	{
			m.heap.erase(i.getWStr());
	}
	++m.instruct_number;
}

bool InstructFREE::validate(Machine& m) {

	if (this->values.size() > 0) {
		for (auto& i : this->values)
		{
			if (i.toSTR().slice(0, 1).getWStr() == L"$") {
				if (m.heap.find(i.getWStr()) == m.heap.end()) {
					throw wstring{ to_wstring(m.instruct_number + 1) + L": Переменная " + i.getWStr() + L" не существует\n" };
				}
			}
			else {
				throw wstring{ to_wstring(m.instruct_number + 1) + L": Параметр " + i.getWStr() + L" должен быть именем переменной\n" };
			}
		}
		return true;
	}
	else {
		throw wstring{ to_wstring(m.instruct_number + 1) + L": Инструкция FREE принимает как минимум 1 параметр\n" };
	}
}
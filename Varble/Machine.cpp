#include "Machine.h"

Machine::Machine(map<wstring, Var> in) {
	this->in_data = in;
}

void Machine::prepare() {
	this->instruct_number = 0;
	for (int i = 0; i < instructions.size(); ++i) {
		if (this->instructions[i]->name == L"LBL") {
			if (this->instructions[i]->values.size() == 1) {
				if (this->instructions[i]->values[0].toSTR().slice(0, 1).getWStr() == L"&") {
					if (this->jmp_pointers.find(this->instructions[i]->values[0].getWStr()) == this->jmp_pointers.end()) {
						this->jmp_pointers[this->instructions[i]->values[0].toSTR().getWStr()] = i;
					}
					else {
						throw wstring{ L"������ " + this->instructions[i]->values[0].toSTR().getWStr() + L" ���������� LBL ��� ����������\n" };
					}
				}
				else {
					throw wstring{ L"�������� " + this->instructions[i]->values[0].toSTR().getWStr() + L" ���������� LBL ������ ���� ������ ������\n"};
				}
			}
			else {
				throw wstring{ L"���������� LBL ��������� ������ 1 ��������\n" };
			}
		}
	}
}

Var Machine::go() {
	while (this->instruct_number != -1) {
		wcout << this->instruct_number << L": " << this->instructions[this->instruct_number]->name << endl;
		wcout << L"-------------------------------------------------------" << endl;
		wcout << L"Heap:" << endl;
		for (const auto& kv : this->heap) {
			wcout << kv.first << ": " << kv.second << endl;
		}
		wcout << L"-------------------------------------------------------" << endl;
		wcout << L"Jump pointers:" << endl;
		for (const auto& kv : this->jmp_pointers) {
			wcout << kv.first << ": " << kv.second << endl;
		}
		wcout << L"=======================================================" << endl;

		if (this->instructions[this->instruct_number]->validate(*this)) {
			this->instructions[this->instruct_number]->go(*this);
		}
	}
	return this->ret_data;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
		throw wstring{ to_wstring(m.instruct_number + 1) + L": ���������� NOP ��������� 0 ����������\n" };
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
				throw wstring{ to_wstring(m.instruct_number + 1) + L": ���������� " + this->values[0].getWStr() + L" �� ����������\n" };
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
		throw wstring{ to_wstring(m.instruct_number + 1) + L": ���������� END ��������� 1 ��������\n" };
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
				throw wstring{ to_wstring(m.instruct_number + 1) + L": ���������� " + this->values[0].getWStr() + L" ��� ����������\n"};
			}
		}
		else {
			throw wstring{ to_wstring(m.instruct_number + 1) + L": ������ �������� ���������� VAR ������ ���� ������ ����������\n" };
		}
	}
	else {
		throw wstring{ to_wstring(m.instruct_number + 1) + L": ���������� VAR ��������� 2 ���������\n" };
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
					throw wstring{ to_wstring(m.instruct_number + 1) + L": ���������� " + i.getWStr() + L" �� ����������\n" };
				}
			}
		}
		return true;
	}
	else {
		throw wstring{ to_wstring(m.instruct_number + 1) + L": ���������� PRINT ��������� ��� ������� 1 ��������\n" };
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
					throw wstring{ to_wstring(m.instruct_number + 1) + L": ���������� " + i.getWStr() + L" �� ����������\n" };
				}
			}
			else {
				throw wstring{ to_wstring(m.instruct_number + 1) + L": �������� " + i.getWStr() + L" ������ ���� ������ ����������\n" };
			}
		}
		return true;
	}
	else {
		throw wstring{ to_wstring(m.instruct_number + 1) + L": ���������� FREE ��������� ��� ������� 1 ��������\n" };
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InstructLBL::InstructLBL(vector<Var> val) {
	this->name = L"LBL";
	this->values = val;
}

void InstructLBL::go(Machine& m) {
	++m.instruct_number;
}

bool InstructLBL::validate(Machine& m) {
	if (this->values.size() == 1) {
		if (this->values[0].toSTR().slice(0, 1).getWStr() == L"&") {
			return true;
		}
		else {
			throw wstring{ to_wstring(m.instruct_number + 1) + L": �������� ���������� LBL ������ ���� ������ ������\n" };
		}
	}
	else {
		throw wstring{ to_wstring(m.instruct_number + 1) + L": ���������� LBL ��������� 1 ��������\n" };
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InstructJMP::InstructJMP(vector<Var> val) {
	this->name = L"JMP";
	this->values = val;
}

void InstructJMP::go(Machine& m) {
	m.instruct_number = m.jmp_pointers[this->values[0].toSTR().getWStr()];
}

bool InstructJMP::validate(Machine& m) {
	if (this->values.size() == 1) {
		if (this->values[0].toSTR().slice(0, 1).getWStr() == L"&") {
			if (m.jmp_pointers.find(this->values[0].getWStr()) == m.jmp_pointers.end()) {
				throw wstring{ to_wstring(m.instruct_number + 1) + L": ������ " + this->values[0].getWStr() + L" �� ����������\n" };
			}
			else {
				return true;
			}
		}
		else {
			throw wstring{ to_wstring(m.instruct_number + 1) + L": �������� ���������� JMP ������ ���� ������ ������\n" };
		}
	}
	else {
		throw wstring{ to_wstring(m.instruct_number + 1) + L": ���������� JMP ��������� 1 ��������\n" };
	}
}
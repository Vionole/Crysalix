#pragma once
#include <map>
#include <vector>

#include "Machine.h"
#include "InstructionFunctions.h"

using namespace std;

Machine::Machine(map<wstring, Var> in, bool dbg) {
	this->in_data = in;
	this->debug = dbg;
}

void Machine::prepare() {
	this->instruct_count = instructions.size();
	this->instruct_number = 0;
	for (int i = 0; i < this->instruct_count; ++i) {
		try {
			functions[this->instructions[i].opCode](this, &instructions[i], true, true);
		}
		catch (const std::wstring& error_message) {
			throw wstring{ L"Ошибка выполнения инструкции " + to_wstring(this->instruct_number + 1) + L"(" + this->instructions[i].as_string + L"): " + error_message };
		}
	}
	this->instruct_number = 0;
}

Var Machine::go() {
	while (this->instruct_number != -1) {
		if (this->instruct_number >= this->instruct_count) {
			throw wstring{ L"Ошибка после выполнения инструкции " + to_wstring(this->instruct_number) + L"(" + this->instructions[this->instruct_number - 1].as_string + L"): " + L"Неожиданный конец программы. Пропущена инструкция END\n" };
		}

		try {
			functions[this->instructions[this->instruct_number].opCode](this, &instructions[this->instruct_number], false, false);
		}
		catch (const std::wstring& error_message) {
				throw wstring{ L"Ошибка выполнения инструкции " + to_wstring(this->instruct_number + 1) + L"(" + this->instructions[this->instruct_number].as_string + L"): " + error_message };
		}
	}
	return this->ret_data;
}
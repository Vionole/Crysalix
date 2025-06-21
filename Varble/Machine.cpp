#include "Machine.h"
#include "windows.h"
#include <cmath>


const wstring error_type = L"Ошибка выполнения инструкции ";
const wstring error_type_2 = L"Ошибка после выполнения инструкции ";

const unsigned char STRICTED = 0;	//Только определенно количество параметров
const unsigned char MIN = 1;		//Минимальное количество параметров
const unsigned char RANGE = 3;		//Параметры от минимального до максимального
const unsigned char VARIANTS = 4;	//Варианты количества параметров

Machine::Machine(map<wstring, Var> in, bool dbg) {
	this->in_data = in;
	this->debug = dbg;
}

void Machine::prepare() {
	this->instruct_count = instructions.size();
	this->instruct_number = 0;
	for (int i = 0; i < this->instruct_count; ++i) {
		if (this->instructions[i]->validate(this, true)) {
			this->instructions[i]->go(this, true);
		}
	}
	this->instruct_number = 0;
}

Var Machine::go() {
	while (this->instruct_number != -1) {
		if (this->instruct_number >= this->instruct_count) {
			throw wstring{ error_type_2 + to_wstring(this->instruct_number) + L": Неожиданный конец программы. Пропущена инструкция END\n" };
		}


		if (this->instructions[this->instruct_number]->validate(this, false)) {
			try {
				this->instructions[this->instruct_number]->go(this, false);
			}
			catch (const std::wstring& error_message) {
				throw wstring{ error_message};
			}
		}
		else {
			throw wstring{ error_type + to_wstring(this->instruct_number + 1) + L": Неизвестная ошибка интерпретации\n" };
		}
	}
	return this->ret_data;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// getValue 
// Возвращает значение параметра по имени переменной или литералу
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Var getValue(Var *val, map<wstring, Var>* heap) {
	if ((*val).type == STR && (*val).getWStr()[0] == L'$') {
		try {
			return (*heap).at((*val).getWStr());
		}
		catch (std::out_of_range& ex) {
			throw wstring{L"Переменная " + (*val).getWStr() + L" не определена\n" };
		}
	}
	else {
		return *val;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// getLabel
// Возвращает значение параметра по имени переменной или литералу
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int getLabel(Var* val, map<wstring, int>* pointers) {
	if ((*val).type == STR && (*val).getWStr()[0] == L'&') {
		try {
			return (*pointers).at((*val).getWStr());
		}
		catch (std::out_of_range& ex) {
			throw wstring{ L"Метка " + (*val).getWStr() + L" не определена\n" };
		}
	}
	return 0;
}


//Хелперы валидации инструкций

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// checkExistValue
// Проверяет существование переменной
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void checkExistValue(Var* val, Machine* m) {
	if ((*val).type == STR && (*val).getWStr()[0] == L'$') {
		if ((*m).heap.find((*val).getWStr()) != (*m).heap.end()) {
			throw wstring{ error_type + to_wstring((*m).instruct_number + 1) + L": Переменная " + (*val).getWStr() + L" уже определена\n" };
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// checkNotExistValue
// Проверяет отсутствие существования переменной
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void checkNotExistValue(Var* val, Machine* m) {
	if ((*val).type == STR && (*val).getWStr()[0] == L'$') {
		if ((*m).heap.find((*val).getWStr()) == (*m).heap.end()) {
			throw wstring{ error_type + to_wstring((*m).instruct_number + 1) + L": Переменная " + (*val).getWStr() + L" не определена\n" };
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// checkExistLabel
// Проверяет существование метки
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void checkExistLabel(Var* val, Machine* m) {
	if ((*val).type == STR && (*val).getWStr()[0] == L'&') {
		if ((*m).jmp_pointers.find((*val).getWStr()) != (*m).jmp_pointers.end()) {
			throw wstring{ error_type + to_wstring((*m).instruct_number + 1) + L": Метка " + (*val).getWStr() + L" уже определена\n" };
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// checkParameterCount
// Проверяет соответствие количества инструкций
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void checkParameterCount(unsigned char type, int count, Machine *m, wstring* name, int stricted = 0, int min = 0, int range[] = nullptr, int variant[] = nullptr) {
	switch (type) {
		case STRICTED:
			if (count != stricted) {
				throw wstring{ error_type + to_wstring((*m).instruct_number + 1) + L": Инструкция " + *name + L" принимает " + to_wstring(stricted) + L" параметр(а)(ов)\n" };
			}
			break;
		case MIN:
			if (count < min) {
				throw wstring{ error_type + to_wstring((*m).instruct_number + 1) + L": Инструкция " + *name + L" принимает не меньше " + to_wstring(min) + L" параметра(ов)\n" };
			}
			break;
		case RANGE:
			if (count < range[0] || count > range[1]) {
				throw wstring{ error_type + to_wstring((*m).instruct_number + 1) + L": Инструкция " + *name + L" принимает от " + to_wstring(range[0]) + L" до " + to_wstring(range[1]) + L" параметра(ов)\n" };
			}
			break;
		case VARIANTS: {
				int size = sizeof(variant) / sizeof(int);
				bool is_variant = false;
				for (int i = 0; i < size; ++i) {
					if (count == variant[i]) {
						is_variant = true;
						break;
					}
				}
				if (!is_variant) {
					wstring params_str = L"";
					for (int i = 0; i < size; ++i) {
						params_str += to_wstring(variant[i]);
						if (i + 1 != size) {
							params_str += L", ";
						}
					}
					throw wstring{ error_type + to_wstring((*m).instruct_number + 1) + L": Инструкция " + *name + L" может принимать следующее число параметров: " + params_str + L"\n" };
				}
			}
			break;
		default:
			throw wstring{ error_type + to_wstring((*m).instruct_number + 1) + L": Указан неверный вариант валидации параметров" };
			break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// requiredVar
// Проверяет то, что требуется переменная
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void requiredVar(Var* val, Machine* m, wstring* type, wstring num) {
	if ((*val).type != STR || (*val).getWStr()[0] != L'$') {
		throw wstring{ error_type + to_wstring((*m).instruct_number + 1) + L": " + num + L" параметр инструкции " + (*type) + L" должен быть именем переменной\n" };
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// requiredLabel
// Проверяет то, что требуется метка
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void requiredLabel(Var* val, Machine* m, wstring* type, wstring num) {
	if ((*val).type != STR || (*val).getWStr()[0] != L'&') {
		throw wstring{ error_type + to_wstring((*m).instruct_number + 1) + L": " + num + L" параметр инструкции " + (*type) + L" должен быть именем метки\n" };
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NOP
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InstructNOP::InstructNOP(vector<Var>* val) {
	this->name = L"NOP";
	this->values = *val;
}

void InstructNOP::go(Machine* m, bool prego = false) {
	++(*m).instruct_number;
}

bool InstructNOP::validate(Machine* m, bool prevalidate = false) {
	if (prevalidate) {
		checkParameterCount(STRICTED, this->values.size(), m, &this->name, 0);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// END
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InstructEND::InstructEND(vector<Var>* val) {
	this->name = L"END";
	this->values = *val;
}

void InstructEND::go(Machine* m, bool prego = false) {
	if (prego) {
		++(*m).instruct_number;
	}
	else {
		(*m).ret_data = getValue(&this->values[0], &(*m).heap);
		(*m).instruct_number = -1;
	}

}

bool InstructEND::validate(Machine* m, bool prevalidate = false) {
	if (prevalidate) {
		checkParameterCount(STRICTED, this->values.size(), m, &this->name, 1);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PAUSE
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InstructPAUSE::InstructPAUSE(vector<Var>* val) {
	this->name = L"PAUSE";
	this->values = *val;
}

void InstructPAUSE::go(Machine* m, bool prego = false) {
	if (prego) {
		++(*m).instruct_number;
	}
	else {
		system("pause");
		++(*m).instruct_number;
	}
}

bool InstructPAUSE::validate(Machine* m, bool prevalidate = false) {
	if (prevalidate) {
		checkParameterCount(STRICTED, this->values.size(), m, &this->name, 0);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SLEEP
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InstructSLEEP::InstructSLEEP(vector<Var>* val) {
	this->name = L"SLEEP";
	this->values = *val;
}

void InstructSLEEP::go(Machine* m, bool prego = false) {
	if (prego) {
		++(*m).instruct_number;
	}
	else {
		Sleep(getValue(&this->values[0], &(*m).heap).toUNTG().getUInt());
		++(*m).instruct_number;
	}
}

bool InstructSLEEP::validate(Machine* m, bool prevalidate = false) {
	if (prevalidate) {
		checkParameterCount(STRICTED, this->values.size(), m, &this->name, 1);
	}
	return true;
}

//////i////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VARIABLE
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InstructVAR::InstructVAR(vector<Var>* val) {
	this->name = L"VAR";
	this->values = *val;
}

void InstructVAR::go(Machine* m, bool prego = false) {
	if (prego) {
		++(*m).instruct_number;
	}
	else {
		(*m).heap[this->values[0].getWStr()] = getValue(&this->values[1], &(*m).heap);
		++(*m).instruct_number;
	}
}

bool InstructVAR::validate(Machine* m, bool prevalidate = false) {
	if (prevalidate) {
		checkParameterCount(STRICTED, this->values.size(), m, &this->name, 2);
		requiredVar(&this->values[0], m, &this->name, L"Первый");
	}
	else {
		checkExistValue(&this->values[0], m);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRINT
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InstructPRINT::InstructPRINT(vector<Var>* val) {
	this->name = L"PRINT";
	this->values = *val;
}

void InstructPRINT::go(Machine* m, bool prego = false) {
	if (prego) {
		++(*m).instruct_number;
	}
	else {
		for (auto& i : this->values)
		{
			wcout << getValue(&i, &(*m).heap);
		}
		++(*m).instruct_number;
	}
}

bool InstructPRINT::validate(Machine* m, bool prevalidate = false) {
	if (prevalidate) {
		checkParameterCount(MIN, this->values.size(), m, &this->name, 0, 1);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FREE
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InstructFREE::InstructFREE(vector<Var>* val) {
	this->name = L"FREE";
	this->values = *val;
}

void InstructFREE::go(Machine* m, bool prego = false) {
	if (prego) {
		++(*m).instruct_number;
	}
	else {
		for (auto& i : this->values)
		{
			(*m).heap.erase(i.getWStr());
		}
		++(*m).instruct_number;
	}
}

bool InstructFREE::validate(Machine* m, bool prevalidate = false) {
	if (prevalidate) {
		checkParameterCount(MIN, this->values.size(), m, &this->name, 0, 1);
		for (auto& i : this->values)
		{
			requiredVar(&i, m, &this->name, i.toSTR().getWStr());
		}
	}
	else {
		for (auto& i : this->values)
		{
			checkNotExistValue(&i, m);
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LABEL
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InstructLABEL::InstructLABEL(vector<Var>* val) {
	this->name = L"LABEL";
	this->values = *val;
}

void InstructLABEL::go(Machine* m, bool prego = false) {
	if (prego) {
		(*m).jmp_pointers[this->values[0].toSTR().getWStr()] = (*m).instruct_number;
		++(*m).instruct_number;
	}
	else {
		++(*m).instruct_number;
	}
}

bool InstructLABEL::validate(Machine* m, bool prevalidate = false) {
	if (prevalidate) {
		checkParameterCount(STRICTED, this->values.size(), m, &this->name, 1);
		requiredLabel(&this->values[0], m, &this->name, L"Единственный");
		checkExistLabel(&this->values[0], m);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// JUMP
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InstructJUMP::InstructJUMP(vector<Var>* val) {
	this->name = L"JUMP";
	this->values = *val;
}

void InstructJUMP::go(Machine* m, bool prego = false) {
	if (prego) {
		++(*m).instruct_number;
	}
	else {
		(*m).instruct_number = getLabel(&this->values[0], &(*m).jmp_pointers) + 1;
	}
}

bool InstructJUMP::validate(Machine* m, bool prevalidate = false) {
	if (prevalidate) {
		checkParameterCount(STRICTED, this->values.size(), m, &this->name, 1);
		requiredLabel(&this->values[0], m, &this->name, L"Единственный");
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// INPUT
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InstructINPUT::InstructINPUT(vector<Var>* val) {
	this->name = L"INPUT";
	this->values = *val;
}

void InstructINPUT::go(Machine* m, bool prego = false) {
	if (prego) {
		++(*m).instruct_number;
	}
	else {
		wstring str;
		getline(wcin, str);
		(*m).heap[this->values[0].getWStr()] = Var(str);
		++(*m).instruct_number;
	}
}

bool InstructINPUT::validate(Machine* m, bool prevalidate = false) {
	if (prevalidate) {
		checkParameterCount(STRICTED, this->values.size(), m, &this->name, 1);
		requiredVar(&this->values[0], m, &this->name, L"Первый");
	}
	else {
		checkNotExistValue(& this->values[0], m);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CHANGE
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InstructCHANGE::InstructCHANGE(vector<Var>* val) {
	this->name = L"CHANGE";
	this->values = *val;
}

void InstructCHANGE::go(Machine* m, bool prego = false) {
	if (prego) {
		++(*m).instruct_number;
	}
	else {
		(*m).heap[this->values[0].getWStr()] = getValue(&this->values[1], &(*m).heap);
		++(*m).instruct_number;
	}
}

bool InstructCHANGE::validate(Machine* m, bool prevalidate = false) {
	if (prevalidate) {
		checkParameterCount(STRICTED, this->values.size(), m, &this->name, 2);
		requiredVar(&this->values[0], m, &this->name, L"Первый");
	}
	else {
		checkNotExistValue(&this->values[0], m);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TO
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InstructTO::InstructTO(vector<Var>* val) {
	this->name = L"TO";
	this->values = *val;
}

void InstructTO::go(Machine* m, bool prego = false) {
	if (prego) {
		++(*m).instruct_number;
	}
	else {
		wstring type = getValue(&this->values[0], &(*m).heap).toSTR().getWStr();

		if (this->values.size() == 2) {
			if (type == L"NTG" || type == L"ntg") {
				(*m).heap[this->values[1].getWStr()] = getValue(&this->values[1], &(*m).heap).toNTG();
			}
			else if (type == L"UNTG" || type == L"untg") {
				(*m).heap[this->values[1].getWStr()] = getValue(&this->values[1], &(*m).heap).toUNTG();
			}
			else if (type == L"DBL" || type == L"dbl") {
				(*m).heap[this->values[1].getWStr()] = getValue(&this->values[1], &(*m).heap).toDBL();
			}
			else if (type == L"CHR" || type == L"chr") {
				(*m).heap[this->values[1].getWStr()] = getValue(&this->values[1], &(*m).heap).toCHR();
			}
			else if (type == L"UCHR" || type == L"uchr") {
				(*m).heap[this->values[1].getWStr()] = getValue(&this->values[1], &(*m).heap).toUCHR();
			}
			else if (type == L"BLN" || type == L"bln") {
				(*m).heap[this->values[1].getWStr()] = getValue(&this->values[1], &(*m).heap).toBLN();
			}
			else if (type == L"STR" || type == L"str") {
				(*m).heap[this->values[1].getWStr()] = getValue(&this->values[1], &(*m).heap).toSTR();
			}
			else if (type == L"ARR" || type == L"arr") {
				(*m).heap[this->values[1].getWStr()] = getValue(&this->values[1], &(*m).heap).toARR();
			}
			else {
				throw wstring{ error_type + to_wstring((*m).instruct_number + 1) + L": Тип данных " + type + L" неизвестен\n" };
			}
		}
		else if (this->values.size() == 3) {
			if (type == L"NTG" || type == L"ntg") {
				(*m).heap[this->values[1].getWStr()] = getValue(&this->values[2], &(*m).heap).toNTG();
			}
			else if (type == L"UNTG" || type == L"untg") {
				(*m).heap[this->values[1].getWStr()] = getValue(&this->values[2], &(*m).heap).toUNTG();
			}
			else if (type == L"DBL" || type == L"dbl") {
				(*m).heap[this->values[1].getWStr()] = getValue(&this->values[2], &(*m).heap).toDBL();
			}
			else if (type == L"CHR" || type == L"chr") {
				(*m).heap[this->values[1].getWStr()] = getValue(&this->values[2], &(*m).heap).toCHR();
			}
			else if (type == L"UCHR" || type == L"uchr") {
				(*m).heap[this->values[1].getWStr()] = getValue(&this->values[2], &(*m).heap).toUCHR();
			}
			else if (type == L"BLN" || type == L"bln") {
				(*m).heap[this->values[1].getWStr()] = getValue(&this->values[2], &(*m).heap).toBLN();
			}
			else if (type == L"STR" || type == L"str") {
				(*m).heap[this->values[1].getWStr()] = getValue(&this->values[2], &(*m).heap).toSTR();
			}
			else if (type == L"ARR" || type == L"arr") {
				(*m).heap[this->values[1].getWStr()] = getValue(&this->values[2], &(*m).heap).toARR();
			}
			else {
				throw wstring{ error_type + to_wstring((*m).instruct_number + 1) + L": Тип данных " + type + L" неизвестен\n" };
			}
		}
		++(*m).instruct_number;
	}
}

bool InstructTO::validate(Machine* m, bool prevalidate = false) {

	if (prevalidate) {
		int v[2]{ 2, 3 };
		checkParameterCount(VARIANTS, this->values.size(), m, &this->name, 0, 0, nullptr, v);
		requiredVar(&this->values[1], m, &this->name, L"Второй");
	}
	else {
		checkNotExistValue(&this->values[1], m);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CALC
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InstructCALC::InstructCALC(vector<Var>* val) {
	this->name = L"CALC";
	this->values = *val;
}

void InstructCALC::go(Machine* m, bool prego = false) {
	if (prego) {
		++(*m).instruct_number;
	}
	else {
		wstring type = getValue(&this->values[0], &(*m).heap).toSTR().getWStr();
		if (type != L"+"
			&& type != L"-"
			&& type != L"*"
			&& type != L"/"
			&& type != L"%"
			&& type != L"^"
			&& type != L"INC"
			&& type != L"DEC"
			&& type != L"LOG"
			&& type != L"LN"
			&& type != L"FACT"
			&& type != L"ROOT"
			&& type != L"inc"
			&& type != L"dec"
			&& type != L"log"
			&& type != L"ln"
			&& type != L"fact"
			&& type != L"root"){
			throw wstring{ error_type + to_wstring((*m).instruct_number + 1) + L": математическая операция " + type + L" неизвестна\n" };
		}
		if (this->values.size() == 2) {
			if (type == L"INC" || type == L"inc") {
				(*m).heap[this->values[1].getWStr()] += Var(1);
			}
			else if (type == L"DEC" || type == L"dec") {
				(*m).heap[this->values[1].getWStr()] -= Var(1);
			}
			else if (type == L"FACT" || type == L"fact") {
				int fact = (*m).heap[this->values[1].getWStr()].toUNTG().getUInt();
				unsigned long long int result = 1;
				for (int i = 1; i <= fact; ++i) {
					result *= i;
				}
				(*m).heap[this->values[1].getWStr()] = Var(result);
			}
			else if (type == L"LN" || type == L"ln") {
				(*m).heap[this->values[1].getWStr()] = Var(log((*m).heap[this->values[1].getWStr()].toDBL().getDouble()));
			}
			else {
				throw wstring{ error_type + to_wstring((*m).instruct_number + 1) + L": математическая операция " + type + L" принимает 2 и больше параметров\n" };
			}
		}
		if (this->values.size() == 3) {
			if (type == L"INC" || type == L"inc") {
				(*m).heap[this->values[1].getWStr()] = getValue(&this->values[2], &(*m).heap) + Var(1);
			}
			else if (type == L"DEC" || type == L"dec") {
				(*m).heap[this->values[1].getWStr()] = getValue(&this->values[2], &(*m).heap) - Var(1);
			}
			else if (type == L"FACT" || type == L"fact") {
				int fact = getValue(&this->values[2], &(*m).heap).toUNTG().getUInt();
				unsigned long long int result = 1;
				for (int i = 1; i <= fact; ++i) {
					result *= i;
				}
				(*m).heap[this->values[1].getWStr()] = Var(result);
			}
			else if (type == L"LN" || type == L"ln") {
				(*m).heap[this->values[1].getWStr()] = Var(log(getValue(&this->values[2], &(*m).heap).toDBL().getDouble()));
			}
			else if (type == L"+") {
				(*m).heap[this->values[1].getWStr()] += getValue(&this->values[2], &(*m).heap);
			}
			else if (type == L"-") {
				(*m).heap[this->values[1].getWStr()] -= getValue(&this->values[2], &(*m).heap);
			}
			else if (type == L"*") {
				(*m).heap[this->values[1].getWStr()] *= getValue(&this->values[2], &(*m).heap);
			}
			else if (type == L"/") {
				(*m).heap[this->values[1].getWStr()] /= getValue(&this->values[2], &(*m).heap);
			}
			else if (type == L"%") {
				(*m).heap[this->values[1].getWStr()] %= getValue(&this->values[2], &(*m).heap);
			}
			else if (type == L"^") {
				(*m).heap[this->values[1].getWStr()] = Var(pow(getValue(&this->values[1], &(*m).heap).toDBL().getDouble(), getValue(&this->values[2], &(*m).heap).toDBL().getDouble()));
			}
			else if (type == L"ROOT" || type == L"root") {
				(*m).heap[this->values[1].getWStr()] = Var(pow(getValue(&this->values[1], &(*m).heap).toDBL().getDouble(), 1.0 / getValue(&this->values[2], &(*m).heap).toDBL().getDouble()));
			}
			else if (type == L"LOG" || type == L"log") {
				(*m).heap[this->values[1].getWStr()] = log(getValue(&this->values[1], &(*m).heap).toDBL().getDouble()) / log(getValue(&this->values[2], &(*m).heap).toDBL().getDouble());

			}
		}
		if (this->values.size() == 4) {
			if (type == L"+") {
				(*m).heap[this->values[1].getWStr()] = getValue(&this->values[2], &(*m).heap) + getValue(&this->values[3], &(*m).heap);
			}
			else if (type == L"-") {
				(*m).heap[this->values[1].getWStr()] = getValue(&this->values[2], &(*m).heap) - getValue(&this->values[3], &(*m).heap);
			}
			else if (type == L"*") {
				(*m).heap[this->values[1].getWStr()] = getValue(&this->values[2], &(*m).heap) * getValue(&this->values[3], &(*m).heap);
			}
			else if (type == L"/") {
				(*m).heap[this->values[1].getWStr()] = getValue(&this->values[2], &(*m).heap) / getValue(&this->values[3], &(*m).heap);
			}
			else if (type == L"%") {
				(*m).heap[this->values[1].getWStr()] = getValue(&this->values[2], &(*m).heap) % getValue(&this->values[3], &(*m).heap);
			}
			else if (type == L"^") {
				(*m).heap[this->values[1].getWStr()] = Var(pow(getValue(&this->values[2], &(*m).heap).toDBL().getDouble(), getValue(&this->values[3], &(*m).heap).toDBL().getDouble()));
			}
			else if (type == L"ROOT" || type == L"root") {
				(*m).heap[this->values[1].getWStr()] = Var(pow(getValue(&this->values[2], &(*m).heap).toDBL().getDouble(), 1.0 / getValue(&this->values[3], &(*m).heap).toDBL().getDouble()));
			}
			else if (type == L"LOG" || type == L"log") {
				(*m).heap[this->values[1].getWStr()] = log(getValue(&this->values[2], &(*m).heap).toDBL().getDouble()) / log(getValue(&this->values[3], &(*m).heap).toDBL().getDouble());

			}
			else {
				throw wstring{ error_type + to_wstring((*m).instruct_number + 1) + L": математическая операция " + type + L" принимает до 3 параметров\n" };
			}
		}
		
		++(*m).instruct_number;
	}
}

bool InstructCALC::validate(Machine* m, bool prevalidate = false) {

	if (prevalidate) {
		int v[2]{ 2, 4 };
		checkParameterCount(RANGE, this->values.size(), m, &this->name, 0, 0, v);
		requiredVar(&this->values[1], m, &this->name, L"Второй");

	}
	else {
		checkNotExistValue(&this->values[1], m);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NEWTEMP
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InstructNEWTEMP::InstructNEWTEMP(vector<Var>* val) {
	this->name = L"NEWTEMP";
	this->values = *val;
}

void InstructNEWTEMP::go(Machine* m, bool prego = false) {
	if (prego) {
		++(*m).instruct_number;
	}
	else {
		int start = (*m).tmp_count;
		(*m).tmp_count += getValue(&(this->values[0]), &(*m).heap).toUNTG().getUInt();
		for (int i = start; i < (*m).tmp_count; ++i) {
			(*m).heap[L"$" + to_wstring(i)] = Var();
		}
		++(*m).instruct_number;
	}
}

bool InstructNEWTEMP::validate(Machine* m, bool prevalidate = false) {
	if (prevalidate) {
		checkParameterCount(STRICTED, this->values.size(), m, &this->name, 1);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FORGET
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InstructFORGET::InstructFORGET(vector<Var>* val) {
	this->name = L"FORGET";
	this->values = *val;
}

void InstructFORGET::go(Machine* m, bool prego = false) {
	if (prego) {
		++(*m).instruct_number;
	}
	else {
		if (this->values.size() == 0) {
			int start = (*m).tmp_count - 1;
			(*m).tmp_count = 0;
			for (int i = start; i >= (*m).tmp_count; --i) {
				(*m).heap.erase(L"$" + to_wstring(i));
			}
		}
		else if (this->values.size() == 1) {
			int start = (*m).tmp_count - 1;
			(*m).tmp_count -= getValue(&(this->values[0]), &(*m).heap).toUNTG().getUInt();
			for (int i = start; i >= (*m).tmp_count; --i) {
				(*m).heap.erase(L"$" + to_wstring(i));
			}
		}
		++(*m).instruct_number;
	}
}

bool InstructFORGET::validate(Machine* m, bool prevalidate = false) {
	if (prevalidate) {
		int v[2]{ 0, 1};
		checkParameterCount(VARIANTS, this->values.size(), m, &this->name, 0, 0, nullptr, v);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TCOUNT
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InstructTCOUNT::InstructTCOUNT(vector<Var>* val) {
	this->name = L"TCOUNT";
	this->values = *val;
}

void InstructTCOUNT::go(Machine* m, bool prego = false) {
	if (prego) {
		++(*m).instruct_number;
	}
	else {
		(*m).heap[this->values[0].toSTR().getWStr()] = Var((*m).tmp_count);
		++(*m).instruct_number;
	}
}

bool InstructTCOUNT::validate(Machine* m, bool prevalidate = false) {
	if (prevalidate) {
		checkParameterCount(STRICTED, this->values.size(), m, &this->name, 1);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ISSET
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
InstructISSET::InstructISSET(vector<Var>* val) {
	this->name = L"ISSET";
	this->values = *val;
}

void InstructISSET::go(Machine* m, bool prego = false) {
	if (prego) {
		++(*m).instruct_number;
	}
	else {
		(*m).heap[this->values[0].toSTR().getWStr()] = Var((*m).tmp_count);
		++(*m).instruct_number;
	}
}

bool InstructISSET::validate(Machine* m, bool prevalidate = false) {
	if (prevalidate) {
		checkParameterCount(STRICTED, this->values.size(), m, &this->name, 2);
	}
	return true;
}
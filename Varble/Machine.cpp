#include "Machine.h"
#include "windows.h"
#include <cmath>


const wstring error_type = L"Ошибка выполнения инструкции ";
const wstring error_type_2 = L"Ошибка после выполнения инструкции ";

const unsigned char STRICTED = 0;	//Только определенно количество параметров
const unsigned char MIN = 1;		//Минимальное количество параметров
const unsigned char RANGE = 3;		//Параметры от минимального до максимального
const unsigned char VARIANTS = 4;	//Варианты количества параметров

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Прототипы функций - инструкций
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void nop(Machine* m, Instruction* i, bool prevalidate, bool prego);
void end(Machine* m, Instruction* i, bool prevalidate, bool prego);
void pause(Machine* m, Instruction* i, bool prevalidate, bool prego);
void sleep(Machine* m, Instruction* i, bool prevalidate, bool prego);
void var(Machine* m, Instruction* i, bool prevalidate, bool prego);
void print(Machine* m, Instruction* i, bool prevalidate, bool prego);
void free(Machine* m, Instruction* i, bool prevalidate, bool prego);
void label(Machine* m, Instruction* i, bool prevalidate, bool prego);
void jump(Machine* m, Instruction* i, bool prevalidate, bool prego);
void input(Machine* m, Instruction* i, bool prevalidate, bool prego);
void change(Machine* m, Instruction* i, bool prevalidate, bool prego);
void to(Machine* m, Instruction* i, bool prevalidate, bool prego);
void calc(Machine* m, Instruction* i, bool prevalidate, bool prego);
void newtemp(Machine* m, Instruction* i, bool prevalidate, bool prego);
void forget(Machine* m, Instruction* i, bool prevalidate, bool prego);
void tcount(Machine* m, Instruction* i, bool prevalidate, bool prego);
void isset(Machine* m, Instruction* i, bool prevalidate, bool prego);
void typeof(Machine* m, Instruction* i, bool prevalidate, bool prego);
void comp(Machine* m, Instruction* i, bool prevalidate, bool prego);
void logic(Machine* m, Instruction* i, bool prevalidate, bool prego);
void jif(Machine* m, Instruction* i, bool prevalidate, bool prego);
void jifnot(Machine* m, Instruction* i, bool prevalidate, bool prego);
void dlabel(Machine* m, Instruction* i, bool prevalidate, bool prego);
void swap(Machine* m, Instruction* i, bool prevalidate, bool prego);
void arr(Machine* m, Instruction* i, bool prevalidate, bool prego);
void valstoarr(Machine* m, Instruction* i, bool prevalidate, bool prego);
void pushb(Machine* m, Instruction* i, bool prevalidate, bool prego);
void popb(Machine* m, Instruction* i, bool prevalidate, bool prego);
void pushf(Machine* m, Instruction* i, bool prevalidate, bool prego);
void popf(Machine* m, Instruction* i, bool prevalidate, bool prego);
void erase(Machine* m, Instruction* i, bool prevalidate, bool prego);
void insrt(Machine* m, Instruction* i, bool prevalidate, bool prego);
void clear(Machine* m, Instruction* i, bool prevalidate, bool prego);
void sizearr(Machine* m, Instruction* i, bool prevalidate, bool prego);
void getval(Machine* m, Instruction* i, bool prevalidate, bool prego);
void setval(Machine* m, Instruction* i, bool prevalidate, bool prego);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Массив с функциями - инструкциями
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
func functions[] = {
	&nop,
	&end,
	&pause,
	&sleep,
	&var,
	&print,
	&free,
	&label,
	&jump,
	&input,
	&change,
	&to,
	&calc,
	&newtemp,
	&forget,
	&tcount,
	&isset,
	&typeof,
	&comp,
	&logic,
	&jif,
	&jifnot,
	&dlabel,
	&swap,
	&arr,
	&valstoarr,
	&pushb,
	&popb,
	&pushf,
	&popf,
	&erase,
	&insrt,
	&clear,
	&sizearr,
	&getval,
	&setval
};

Machine::Machine(map<wstring, Var> in, bool dbg) {
	this->in_data = in;
	this->debug = dbg;
}

void Machine::prepare() {
	this->instruct_count = instructions.size();
	this->instruct_number = 0;
	for (int i = 0; i < this->instruct_count; ++i) {
		functions[this->instructions[i].opCode](this, &instructions[i], true, true);
	}
	this->instruct_number = 0;
}

Var Machine::go() {
	while (this->instruct_number != -1) {
		if (this->instruct_number >= this->instruct_count) {
			throw wstring{ error_type_2 + to_wstring(this->instruct_number) + L": Неожиданный конец программы. Пропущена инструкция END\n" };
		}

		try {
			functions[this->instructions[this->instruct_number].opCode](this, &instructions[this->instruct_number], false, false);
		}
		catch (const std::wstring& error_message) {
				throw wstring{ error_message };
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
Var getLabel(Var* val, map<wstring, int>* pointers) {
	if ((*val).type == STR && (*val).getWStr()[0] == L'&') {
		try {
			return (*pointers).at((*val).getWStr());
		}
		catch (std::out_of_range& ex) {
			throw wstring{ L"Метка " + (*val).getWStr() + L" не определена\n" };
		}
	}
	else {
		return *val;
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
void nop(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"NOP";
		checkParameterCount(STRICTED, (*i).parameters.size(), m, &name, 0);
	}
	else {
		//Ничего
	}

	if (prego) {
		//Ничего
	}
	else {
		++(*m).instruct_number;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// END
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void end(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"END";
		checkParameterCount(STRICTED, (*i).parameters.size(), m, &name, 1);
	}
	else {
		//Ничего
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		(*m).ret_data = getValue(&(*i).parameters[0], &(*m).heap);
		(*m).instruct_number = -1;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PAUSE
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void pause(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"PAUSE";
		checkParameterCount(STRICTED, (*i).parameters.size(), m, &name, 0);
	}
	else {
		//Ничего
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		system("pause");
		++(*m).instruct_number;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SLEEP
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void sleep(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"SLEEP";
		if (prevalidate) {
			checkParameterCount(STRICTED, (*i).parameters.size(), m, &name, 1);
		}
	}
	else {
		//Ничего
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		Sleep(getValue(&(*i).parameters[0], &(*m).heap).toUNTG().getUInt());
		++(*m).instruct_number;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VAR
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void var(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"VAR";
		checkParameterCount(STRICTED, (*i).parameters.size(), m, &name, 2);
		requiredVar(&(*i).parameters[0], m, &name, L"Первый");
	}
	else {
		checkExistValue(&(*i).parameters[0], m);
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		(*m).heap[(*i).parameters[0].getWStr()] = getValue(&(*i).parameters[1], &(*m).heap);
		++(*m).instruct_number;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PRINT
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void print(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"PRINT";
		checkParameterCount(MIN, (*i).parameters.size(), m, &name, 0, 1);
	}
	else {
		//Ничего
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		for (Var& v : (*i).parameters)
		{
			wcout << getValue(&v, &(*m).heap);
		}
		++(*m).instruct_number;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FREE
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void free(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"FREE";
		checkParameterCount(MIN, (*i).parameters.size(), m, &name, 0, 1);
		for (Var& i : (*i).parameters)
		{
			requiredVar(&i, m, &name, i.toSTR().getWStr());
		}
	}
	else {

	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		for (Var& v : (*i).parameters)
		{
			(*m).heap.erase(v.getWStr());
		}
		++(*m).instruct_number;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LABEL
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void label(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"LABEL";
		checkParameterCount(STRICTED, (*i).parameters.size(), m, &name, 1);
		requiredLabel(&(*i).parameters[0], m, &name, L"Единственный");
		checkExistLabel(&(*i).parameters[0], m);
	}
	else {
		//Ничего
	}

	if (prego) {
		(*m).jmp_pointers[(*i).parameters[0].toSTR().getWStr()] = (*m).instruct_number + 1;
		++(*m).instruct_number;
	}
	else {
		++(*m).instruct_number;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// JUMP
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void jump(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"JUMP";
		checkParameterCount(STRICTED, (*i).parameters.size(), m, &name, 1);
	}
	else {
		//Ничего
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		(*m).instruct_number = getLabel(&(*i).parameters[0], &(*m).jmp_pointers).toUNTG().getUInt();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// INPUT
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void input(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"INPUT";
		checkParameterCount(STRICTED, (*i).parameters.size(), m, &name, 1);
		requiredVar(&(*i).parameters[0], m, &name, L"Первый");
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		wstring str;
		getline(wcin, str);
		(*m).heap[(*i).parameters[0].getWStr()] = Var(str);
		++(*m).instruct_number;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CHANGE
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void change(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"CHANGE";
		checkParameterCount(STRICTED, (*i).parameters.size(), m, &name, 2);
		requiredVar(&(*i).parameters[0], m, &name, L"Первый");
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		(*m).heap[(*i).parameters[0].getWStr()] = getValue(&(*i).parameters[1], &(*m).heap);
		++(*m).instruct_number;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TO
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void to(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"TO";
		int v[2]{ 2, 3 };
		checkParameterCount(VARIANTS, (*i).parameters.size(), m, &name, 0, 0, nullptr, v);
		requiredVar(&(*i).parameters[1], m, &name, L"Второй");
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		wstring type = getValue(&(*i).parameters[0], &(*m).heap).toSTR().getWStr();

		if ((*i).parameters.size() == 2) {
			if (type == L"NTG" || type == L"ntg") {
				(*m).heap[(*i).parameters[1].getWStr()] = getValue(&(*i).parameters[1], &(*m).heap).toNTG();
			}
			else if (type == L"UNTG" || type == L"untg") {
				(*m).heap[(*i).parameters[1].getWStr()] = getValue(&(*i).parameters[1], &(*m).heap).toUNTG();
			}
			else if (type == L"DBL" || type == L"dbl") {
				(*m).heap[(*i).parameters[1].getWStr()] = getValue(&(*i).parameters[1], &(*m).heap).toDBL();
			}
			else if (type == L"CHR" || type == L"chr") {
				(*m).heap[(*i).parameters[1].getWStr()] = getValue(&(*i).parameters[1], &(*m).heap).toCHR();
			}
			else if (type == L"UCHR" || type == L"uchr") {
				(*m).heap[(*i).parameters[1].getWStr()] = getValue(&(*i).parameters[1], &(*m).heap).toUCHR();
			}
			else if (type == L"BLN" || type == L"bln") {
				(*m).heap[(*i).parameters[1].getWStr()] = getValue(&(*i).parameters[1], &(*m).heap).toBLN();
			}
			else if (type == L"STR" || type == L"str") {
				(*m).heap[(*i).parameters[1].getWStr()] = getValue(&(*i).parameters[1], &(*m).heap).toSTR();
			}
			else if (type == L"ARR" || type == L"arr") {
				(*m).heap[(*i).parameters[1].getWStr()] = getValue(&(*i).parameters[1], &(*m).heap).toARR();
			}
			else {
				throw wstring{ error_type + to_wstring((*m).instruct_number + 1) + L": Тип данных " + type + L" неизвестен\n" };
			}
		}
		else if ((*i).parameters.size() == 3) {
			if (type == L"NTG" || type == L"ntg") {
				(*m).heap[(*i).parameters[1].getWStr()] = getValue(&(*i).parameters[2], & (*m).heap).toNTG();
			}
			else if (type == L"UNTG" || type == L"untg") {
				(*m).heap[(*i).parameters[1].getWStr()] = getValue(&(*i).parameters[2], &(*m).heap).toUNTG();
			}
			else if (type == L"DBL" || type == L"dbl") {
				(*m).heap[(*i).parameters[1].getWStr()] = getValue(&(*i).parameters[2], &(*m).heap).toDBL();
			}
			else if (type == L"CHR" || type == L"chr") {
				(*m).heap[(*i).parameters[1].getWStr()] = getValue(&(*i).parameters[2], &(*m).heap).toCHR();
			}
			else if (type == L"UCHR" || type == L"uchr") {
				(*m).heap[(*i).parameters[1].getWStr()] = getValue(&(*i).parameters[2], &(*m).heap).toUCHR();
			}
			else if (type == L"BLN" || type == L"bln") {
				(*m).heap[(*i).parameters[1].getWStr()] = getValue(&(*i).parameters[2], &(*m).heap).toBLN();
			}
			else if (type == L"STR" || type == L"str") {
				(*m).heap[(*i).parameters[1].getWStr()] = getValue(&(*i).parameters[2], &(*m).heap).toSTR();
			}
			else if (type == L"ARR" || type == L"arr") {
				(*m).heap[(*i).parameters[1].getWStr()] = getValue(&(*i).parameters[2], &(*m).heap).toARR();
			}
			else {
				throw wstring{ error_type + to_wstring((*m).instruct_number + 1) + L": Тип данных " + type + L" неизвестен\n" };
			}
		}
		++(*m).instruct_number;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CALC
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void calc(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"CALC";
		int v[2]{ 2, 4 };
		checkParameterCount(RANGE, (*i).parameters.size(), m, &name, 0, 0, v);
		requiredVar(&(*i).parameters[1], m, &name, L"Второй");

	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		wstring type = getValue(&(*i).parameters[0], &(*m).heap).toSTR().getWStr();
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
			&& type != L"root") {
			throw wstring{ error_type + to_wstring((*m).instruct_number + 1) + L": математическая операция " + type + L" неизвестна\n" };
		}
		if ((*i).parameters.size() == 2) {
			if (type == L"INC" || type == L"inc") {
				(*m).heap[(*i).parameters[1].getWStr()] += Var(1);
			}
			else if (type == L"DEC" || type == L"dec") {
				(*m).heap[(*i).parameters[1].getWStr()] -= Var(1);
			}
			else if (type == L"FACT" || type == L"fact") {
				int fact = (*m).heap[(*i).parameters[1].getWStr()].toUNTG().getUInt();
				unsigned long long int result = 1;
				for (int i = 1; i <= fact; ++i) {
					result *= i;
				}
				(*m).heap[(*i).parameters[1].getWStr()] = Var(result);
			}
			else if (type == L"LN" || type == L"ln") {
				(*m).heap[(*i).parameters[1].getWStr()] = Var(log((*m).heap[(*i).parameters[1].getWStr()].toDBL().getDouble()));
			}
			else {
				throw wstring{ error_type + to_wstring((*m).instruct_number + 1) + L": математическая операция " + type + L" принимает 2 и больше параметров\n" };
			}
		}
		if ((*i).parameters.size() == 3) {
			if (type == L"INC" || type == L"inc") {
				(*m).heap[(*i).parameters[1].getWStr()] = getValue(&(*i).parameters[2], &(*m).heap) + Var(1);
			}
			else if (type == L"DEC" || type == L"dec") {
				(*m).heap[(*i).parameters[1].getWStr()] = getValue(&(*i).parameters[2], &(*m).heap) - Var(1);
			}
			else if (type == L"FACT" || type == L"fact") {
				int fact = getValue(&(*i).parameters[2], &(*m).heap).toUNTG().getUInt();
				unsigned long long int result = 1;
				for (int i = 1; i <= fact; ++i) {
					result *= i;
				}
				(*m).heap[(*i).parameters[1].getWStr()] = Var(result);
			}
			else if (type == L"LN" || type == L"ln") {
				(*m).heap[(*i).parameters[1].getWStr()] = Var(log(getValue(&(*i).parameters[2], &(*m).heap).toDBL().getDouble()));
			}
			else if (type == L"+") {
				(*m).heap[(*i).parameters[1].getWStr()] += getValue(&(*i).parameters[2], &(*m).heap);
			}
			else if (type == L"-") {
				(*m).heap[(*i).parameters[1].getWStr()] -= getValue(&(*i).parameters[2], &(*m).heap);
			}
			else if (type == L"*") {
				(*m).heap[(*i).parameters[1].getWStr()] *= getValue(&(*i).parameters[2], &(*m).heap);
			}
			else if (type == L"/") {
				(*m).heap[(*i).parameters[1].getWStr()] /= getValue(&(*i).parameters[2], &(*m).heap);
			}
			else if (type == L"%") {
				(*m).heap[(*i).parameters[1].getWStr()] %= getValue(&(*i).parameters[2], &(*m).heap);
			}
			else if (type == L"^") {
				(*m).heap[(*i).parameters[1].getWStr()] = Var(pow(getValue(&(*i).parameters[1], &(*m).heap).toDBL().getDouble(), getValue(&(*i).parameters[2], &(*m).heap).toDBL().getDouble()));
			}
			else if (type == L"ROOT" || type == L"root") {
				(*m).heap[(*i).parameters[1].getWStr()] = Var(pow(getValue(&(*i).parameters[1], &(*m).heap).toDBL().getDouble(), 1.0 / getValue(&(*i).parameters[2], &(*m).heap).toDBL().getDouble()));
			}
			else if (type == L"LOG" || type == L"log") {
				(*m).heap[(*i).parameters[1].getWStr()] = log(getValue(&(*i).parameters[1], &(*m).heap).toDBL().getDouble()) / log(getValue(&(*i).parameters[2], &(*m).heap).toDBL().getDouble());

			}
		}
		if ((*i).parameters.size() == 4) {
			if (type == L"+") {
				(*m).heap[(*i).parameters[1].getWStr()] = getValue(&(*i).parameters[2], &(*m).heap) + getValue(&(*i).parameters[3], &(*m).heap);
			}
			else if (type == L"-") {
				(*m).heap[(*i).parameters[1].getWStr()] = getValue(&(*i).parameters[2], &(*m).heap) - getValue(&(*i).parameters[3], &(*m).heap);
			}
			else if (type == L"*") {
				(*m).heap[(*i).parameters[1].getWStr()] = getValue(&(*i).parameters[2], &(*m).heap) * getValue(&(*i).parameters[3], &(*m).heap);
			}
			else if (type == L"/") {
				(*m).heap[(*i).parameters[1].getWStr()] = getValue(&(*i).parameters[2], &(*m).heap) / getValue(&(*i).parameters[3], &(*m).heap);
			}
			else if (type == L"%") {
				(*m).heap[(*i).parameters[1].getWStr()] = getValue(&(*i).parameters[2], &(*m).heap) % getValue(&(*i).parameters[3], &(*m).heap);
			}
			else if (type == L"^") {
				(*m).heap[(*i).parameters[1].getWStr()] = Var(pow(getValue(&(*i).parameters[2], &(*m).heap).toDBL().getDouble(), getValue(&(*i).parameters[3], &(*m).heap).toDBL().getDouble()));
			}
			else if (type == L"ROOT" || type == L"root") {
				(*m).heap[(*i).parameters[1].getWStr()] = Var(pow(getValue(&(*i).parameters[2], &(*m).heap).toDBL().getDouble(), 1.0 / getValue(&(*i).parameters[3], &(*m).heap).toDBL().getDouble()));
			}
			else if (type == L"LOG" || type == L"log") {
				(*m).heap[(*i).parameters[1].getWStr()] = log(getValue(&(*i).parameters[2], &(*m).heap).toDBL().getDouble()) / log(getValue(&(*i).parameters[3], &(*m).heap).toDBL().getDouble());

			}
			else {
				throw wstring{ error_type + to_wstring((*m).instruct_number + 1) + L": математическая операция " + type + L" принимает до 3 параметров\n" };
			}
		}

		++(*m).instruct_number;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NEWTEMP
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void newtemp(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"NEWTEMP";
		checkParameterCount(STRICTED, (*i).parameters.size(), m, &name, 1);
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		int start = (*m).tmp_count;
		(*m).tmp_count += getValue(&(*i).parameters[0], & (*m).heap).toUNTG().getUInt();
		for (int it = start; it < (*m).tmp_count; ++it) {
			(*m).heap[L"$" + to_wstring(it)] = Var();
		}
		++(*m).instruct_number;
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FORGET
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void forget(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"FORGET";
		int v[2]{ 0, 1 };
		checkParameterCount(VARIANTS, (*i).parameters.size(), m, &name, 0, 0, nullptr, v);
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		if ((*i).parameters.size() == 0) {
			int start = (*m).tmp_count - 1;
			(*m).tmp_count = 0;
			for (int it = start; it >= (*m).tmp_count; --it) {
				(*m).heap.erase(L"$" + to_wstring(it));
			}
		}
		else if ((*i).parameters.size() == 1) {
			int start = (*m).tmp_count - 1;
			(*m).tmp_count -= getValue(&(*i).parameters[0], &(*m).heap).toUNTG().getUInt();
			for (int it = start; it >= (*m).tmp_count; --it) {
				(*m).heap.erase(L"$" + to_wstring(it));
			}
		}
		++(*m).instruct_number;
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TCOUNT
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void tcount(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"TCOUNT";
		checkParameterCount(STRICTED, (*i).parameters.size(), m, &name, 1);
		requiredVar(&(*i).parameters[0], m, &name, L"Первый");
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		(*m).heap[(*i).parameters[0].toSTR().getWStr()] = Var((*m).tmp_count);
		++(*m).instruct_number;
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ISSET
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void isset(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"ISSET";
		checkParameterCount(STRICTED, (*i).parameters.size(), m, &name, 2);
		requiredVar(&(*i).parameters[0], m, &name, L"Первый");
		requiredVar(&(*i).parameters[1], m, &name, L"Второй");
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		if ((*m).heap.find((*i).parameters[1].toSTR().getWStr()) != (*m).heap.end()) {
			(*m).heap[(*i).parameters[0].toSTR().getWStr()] = Var(true);
		}
		else {
			(*m).heap[(*i).parameters[0].toSTR().getWStr()] = Var(false);
		}
		++(*m).instruct_number;
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TYPEOF
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void typeof(Machine* m, Instruction* i, bool prevalidate, bool prego) {

	if (prevalidate) {
		wstring name = L"TYPEOF";
		checkParameterCount(STRICTED, (*i).parameters.size(), m, &name, 2);
		requiredVar(&(*i).parameters[0], m, &name, L"Первый");
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		(*m).heap[(*i).parameters[0].toSTR().getWStr()] = getValue(&(*i).parameters[1], &(*m).heap).typeOf();
		++(*m).instruct_number;
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// COMP
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void comp(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"COMP";
		checkParameterCount(STRICTED, (*i).parameters.size(), m, &name, 4);
		requiredVar(&(*i).parameters[1], m, &name, L"Второй");
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		wstring type = getValue(&(*i).parameters[0], &(*m).heap).toSTR().getWStr();

		if (type == L"==") {
			(*m).heap[(*i).parameters[1].toSTR().getWStr()] = getValue(&(*i).parameters[2], &(*m).heap) == getValue(&(*i).parameters[3], &(*m).heap);
		}
		else if (type == L"!=") {
			(*m).heap[(*i).parameters[1].toSTR().getWStr()] = getValue(&(*i).parameters[2], &(*m).heap) != getValue(&(*i).parameters[3], &(*m).heap);
		}
		else if (type == L">") {
			(*m).heap[(*i).parameters[1].toSTR().getWStr()] = getValue(&(*i).parameters[2], &(*m).heap) > getValue(&(*i).parameters[3], &(*m).heap);
		}
		else if (type == L"<") {
			(*m).heap[(*i).parameters[1].toSTR().getWStr()] = getValue(&(*i).parameters[2], &(*m).heap) < getValue(&(*i).parameters[3], &(*m).heap);
		}
		else if (type == L">=") {
			(*m).heap[(*i).parameters[1].toSTR().getWStr()] = getValue(&(*i).parameters[2], &(*m).heap) >= getValue(&(*i).parameters[3], &(*m).heap);
		}
		else if (type == L"<=") {
			(*m).heap[(*i).parameters[1].toSTR().getWStr()] = getValue(&(*i).parameters[2], &(*m).heap) <= getValue(&(*i).parameters[3], &(*m).heap);
		}
		else {
			throw wstring{ error_type + to_wstring((*m).instruct_number + 1) + L": операция сравнения " + type + L" неизвестна\n" };
		}
		++(*m).instruct_number;
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LOGIC
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void logic(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"LOGIC";
		int v[2]{ 3, 4 };
		checkParameterCount(VARIANTS, (*i).parameters.size(), m, &name, 0, 0, nullptr, v);
		requiredVar(&(*i).parameters[1], m, &name, L"Второй");

	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		wstring type = getValue(&(*i).parameters[0], &(*m).heap).toSTR().getWStr();
		if (type != L"NOT"
			&& type != L"AND"
			&& type != L"OR"
			&& type != L"NAND"
			&& type != L"NOR"
			&& type != L"XOR"
			&& type != L"XNOR"
			&& type != L"not"
			&& type != L"and"
			&& type != L"or"
			&& type != L"nand"
			&& type != L"nor"
			&& type != L"xor"
			&& type != L"xnor") {
			throw wstring{ error_type + to_wstring((*m).instruct_number + 1) + L": логическая операция " + type + L" неизвестна\n" };
		}
		if ((*i).parameters.size() == 3) {
			if (type == L"NOT" || type == L"not") {
				(*m).heap[(*i).parameters[1].toSTR().getWStr()] = !getValue(&(*i).parameters[2], &(*m).heap).toBLN().getBool();
			}
			else {
				throw wstring{ error_type + to_wstring((*m).instruct_number + 1) + L": логическая операция " + type + L" принимет не менее 4 параметров\n" };
			}
		}
		else if ((*i).parameters.size() == 4) {
			if (type == L"AND" || type == L"and") {
				(*m).heap[(*i).parameters[1].toSTR().getWStr()] = getValue(&(*i).parameters[2], &(*m).heap).toBLN().getBool() && getValue(&(*i).parameters[3], &(*m).heap).toBLN().getBool();
			}
			else if (type == L"OR" || type == L"or") {
				(*m).heap[(*i).parameters[1].toSTR().getWStr()] = getValue(&(*i).parameters[2], &(*m).heap).toBLN().getBool() || getValue(&(*i).parameters[3], &(*m).heap).toBLN().getBool();
			}
			else if (type == L"NAND" || type == L"nand") {
				(*m).heap[(*i).parameters[1].toSTR().getWStr()] = !(getValue(&(*i).parameters[2], &(*m).heap).toBLN().getBool() && getValue(&(*i).parameters[3], &(*m).heap).toBLN().getBool());
			}
			else if (type == L"NOR" || type == L"nor") {
				(*m).heap[(*i).parameters[1].toSTR().getWStr()] = !(getValue(&(*i).parameters[2], &(*m).heap).toBLN().getBool() || getValue(&(*i).parameters[3], &(*m).heap).toBLN().getBool());
			}
			else if (type == L"XOR" || type == L"xor") {
				(*m).heap[(*i).parameters[1].toSTR().getWStr()] = !(getValue(&(*i).parameters[2], &(*m).heap).toBLN().getBool()) != !(getValue(&(*i).parameters[3], &(*m).heap).toBLN().getBool());
			}
			else if (type == L"XNOR" || type == L"xnor") {
				(*m).heap[(*i).parameters[1].toSTR().getWStr()] = getValue(&(*i).parameters[2], &(*m).heap).toBLN().getBool() == getValue(&(*i).parameters[3], &(*m).heap).toBLN().getBool();
			}
			else {
				throw wstring{ error_type + to_wstring((*m).instruct_number + 1) + L": логическая операция " + type + L" принимет не более 3 параметров\n" };
			}
		}
		++(*m).instruct_number;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// JIF
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void jif(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"JIF";
		int v[2]{ 1, 2 };
		checkParameterCount(VARIANTS, (*i).parameters.size(), m, &name, 1, 2, nullptr, v);
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		bool swtch = getValue(&(*i).parameters[0], &(*m).heap).toBLN().getBool();
		if (swtch) {
			(*m).instruct_number = getLabel(&(*i).parameters[1], &(*m).jmp_pointers).toUNTG().getUInt();
		}
		else {
			++(*m).instruct_number;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// JIFNOT
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void jifnot(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"JIFNOT";
		int v[2]{ 1, 2 };
		checkParameterCount(VARIANTS, (*i).parameters.size(), m, &name, 1, 2, nullptr, v);
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		bool swtch = getValue(&(*i).parameters[0], &(*m).heap).toBLN().getBool();
		if (swtch) {
			++(*m).instruct_number;
		}
		else {
			(*m).instruct_number = getLabel(&(*i).parameters[1], &(*m).jmp_pointers).toUNTG().getUInt();
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DLABEL
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void dlabel(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"DLABEL";
		checkParameterCount(STRICTED, (*i).parameters.size(), m, &name, 1);
		requiredLabel(&(*i).parameters[0], m, &name, L"Единственный");
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		(*m).jmp_pointers[(*i).parameters[0].toSTR().getWStr()] = (*m).instruct_number + 1;
		++(*m).instruct_number;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SWAP
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void swap(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"SWAP";
		checkParameterCount(STRICTED, (*i).parameters.size(), m, &name, 2);
		requiredVar(&(*i).parameters[0], m, &name, L"Первый");
		requiredVar(&(*i).parameters[1], m, &name, L"Второй");
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		Var firstval = getValue(&(*i).parameters[0], &(*m).heap);
		Var secondval = getValue(&(*i).parameters[1], &(*m).heap);
		swap(firstval, secondval);

		(*m).heap[(*i).parameters[0].toSTR().getWStr()] = firstval;
		(*m).heap[(*i).parameters[1].toSTR().getWStr()] = secondval;
		++(*m).instruct_number;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ARRAY
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void arr(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"ARRAY";
		checkParameterCount(MIN, (*i).parameters.size(), m, &name, 0, 2);
		requiredVar(&(*i).parameters[0], m, &name, L"Первый");
	}
	else {
		checkExistValue(&(*i).parameters[0], m);
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		int dimensions = (*i).parameters.size() - 1;
		Var result = Var();
		for (int iter = dimensions; iter > 0; --iter) {
			int dimension = getValue(&(*i).parameters[iter], &(*m).heap).toUNTG().getUInt();
			vector<Var> v;
			Var arr = Var(v);
			for (int j = 0; j < dimension; ++j) {
				arr.pushb(result);
			}
			result = arr;
		}
		
		(*m).heap[(*i).parameters[0].getWStr()] = result;
		++(*m).instruct_number;
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VALSTOARR
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void valstoarr(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"VALSTOARR";
		checkParameterCount(MIN, (*i).parameters.size(), m, &name, 0, 2);
		requiredVar(&(*i).parameters[0], m, &name, L"Первый");
	}
	else {
		checkExistValue(&(*i).parameters[0], m);
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		int size = (*i).parameters.size() - 1;
		vector<Var> v;
		Var arr = Var(v);
		for (int iter = 1; iter <= size; ++iter) {
			arr.pushb(getValue(&(*i).parameters[iter], &(*m).heap));
		}

		(*m).heap[(*i).parameters[0].getWStr()] = arr;
		++(*m).instruct_number;
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PUSHB
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void pushb(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"PUSHB";
		checkParameterCount(STRICTED, (*i).parameters.size(), m, &name, 2);
		requiredVar(&(*i).parameters[0], m, &name, L"Первый");
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		(*m).heap[(*i).parameters[0].toSTR().getWStr()].pushb(getValue(&(*i).parameters[1], &(*m).heap));
		++(*m).instruct_number;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// POPB
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void popb(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"POPB";
		int v[2]{ 1, 2 };
		checkParameterCount(VARIANTS, (*i).parameters.size(), m, &name, 0, 0, nullptr, v);
		requiredVar(&(*i).parameters[0], m, &name, L"Первый");
		if ((*i).parameters.size() == 2) {
			requiredVar(&(*i).parameters[1], m, &name, L"Второй");
		}
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		if ((*i).parameters.size() == 1) {
			(*m).heap[(*i).parameters[0].toSTR().getWStr()].popb();
			++(*m).instruct_number;
		}
		else {
			(*m).heap[(*i).parameters[0].toSTR().getWStr()] = (*m).heap[(*i).parameters[1].toSTR().getWStr()].popb();
			++(*m).instruct_number;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PUSHF
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void pushf(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"PUSHF";
		checkParameterCount(STRICTED, (*i).parameters.size(), m, &name, 2);
		requiredVar(&(*i).parameters[0], m, &name, L"Первый");
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		(*m).heap[(*i).parameters[0].toSTR().getWStr()].pushf(getValue(&(*i).parameters[1], &(*m).heap));
		++(*m).instruct_number;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// POPF
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void popf(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"POPF";
		int v[2]{ 1, 2 };
		checkParameterCount(VARIANTS, (*i).parameters.size(), m, &name, 0, 0, nullptr, v);
		requiredVar(&(*i).parameters[0], m, &name, L"Первый");
		if ((*i).parameters.size() == 2) {
			requiredVar(&(*i).parameters[1], m, &name, L"Второй");
		}
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		if ((*i).parameters.size() == 1) {
			(*m).heap[(*i).parameters[0].toSTR().getWStr()].popf();
			++(*m).instruct_number;
		}
		else {
			(*m).heap[(*i).parameters[0].toSTR().getWStr()] = (*m).heap[(*i).parameters[1].toSTR().getWStr()].popf();
			++(*m).instruct_number;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ERASE
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void erase(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"ERASE";
		int v[2]{ 2, 3 };
		checkParameterCount(VARIANTS, (*i).parameters.size(), m, &name, 0, 0, nullptr, v);
		requiredVar(&(*i).parameters[0], m, &name, L"Первый");
		if ((*i).parameters.size() == 3) {
			requiredVar(&(*i).parameters[1], m, &name, L"Второй");
		}
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		if ((*i).parameters.size() == 2) {
			(*m).heap[(*i).parameters[0].toSTR().getWStr()].erase(getValue(&(*i).parameters[1], &(*m).heap));
			++(*m).instruct_number;
		}
		else {
			(*m).heap[(*i).parameters[0].toSTR().getWStr()] = (*m).heap[(*i).parameters[1].toSTR().getWStr()].erase(getValue(&(*i).parameters[2], &(*m).heap));
			++(*m).instruct_number;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// INSERT
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void insrt(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"INSERT";
		checkParameterCount(STRICTED, (*i).parameters.size(), m, &name, 3);
		requiredVar(&(*i).parameters[0], m, &name, L"Первый");
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		(*m).heap[(*i).parameters[0].toSTR().getWStr()].insert_vector(getValue(&(*i).parameters[2], &(*m).heap), getValue(&(*i).parameters[1], &(*m).heap));
		++(*m).instruct_number;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLEAR
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void clear(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"CLEAR";
		checkParameterCount(STRICTED, (*i).parameters.size(), m, &name, 1);
		requiredVar(&(*i).parameters[0], m, &name, L"Единственный");
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		(*m).heap[(*i).parameters[0].toSTR().getWStr()].clear();
		++(*m).instruct_number;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SIZE
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void sizearr(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"SIZE";
		checkParameterCount(STRICTED, (*i).parameters.size(), m, &name, 2);
		requiredVar(&(*i).parameters[0], m, &name, L"Первый");
		requiredVar(&(*i).parameters[1], m, &name, L"Второй");
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		(*m).heap[(*i).parameters[0].toSTR().getWStr()] = (*m).heap[(*i).parameters[1].toSTR().getWStr()].csize();
		++(*m).instruct_number;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GETVAL
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void getval(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"GETVAL";
		checkParameterCount(MIN, (*i).parameters.size(), m, &name, 0, 3);
		requiredVar(&(*i).parameters[0], m, &name, L"Первый");
		requiredVar(&(*i).parameters[1], m, &name, L"Второй");
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {
		int dimensions = (*i).parameters.size() - 2;
		Var result = (*m).heap[(*i).parameters[1].toSTR().getWStr()];
		for (int iter = 0; iter  < dimensions; ++iter) {
			int dimension = getValue(&(*i).parameters[iter + 2], &(*m).heap).toUNTG().getUInt();
			Var res = result[dimension];
			result = res;
		}
		(*m).heap[(*i).parameters[0].toSTR().getWStr()] = result;
		++(*m).instruct_number;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SETVAL
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setval(Machine* m, Instruction* i, bool prevalidate, bool prego) {
	if (prevalidate) {
		wstring name = L"SETVAL";
		checkParameterCount(MIN, (*i).parameters.size(), m, &name, 0, 3);
		requiredVar(&(*i).parameters[1], m, &name, L"Второй");
	}

	if (prego) {
		++(*m).instruct_number;
	}
	else {

		int dimensions = (*i).parameters.size() - 2;
		Var* result = &((*m).heap[(*i).parameters[1].toSTR().getWStr()]);

		for (int iter = 0; iter < dimensions; ++iter) {
			int dimension = getValue(&(*i).parameters[iter + 2], &(*m).heap).toUNTG().getUInt();
			if (iter == dimensions - 1) {
				(*result)[getValue(&(*i).parameters[iter + 2], &(*m).heap)] = getValue(&(*i).parameters[0], &(*m).heap);
				break;
			}
			Var* res = &(*result)[dimension];
			result = res;
		}

		++(*m).instruct_number;
	}
}
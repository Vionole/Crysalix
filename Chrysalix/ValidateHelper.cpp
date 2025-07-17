#pragma once
#include <vector>
#include <map>
#include <string>
#include <iostream>

#include "ValidateHelper.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// getValue 
// Возвращает значение параметра по имени переменной или литералу
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Var getValue(Var* val, map<wstring, Var>* heap) {
	if ((*val).type == STR && (*val).getWStr()[0] == L'$') {
		try {
			return (*heap).at((*val).getWStr());
		}
		catch (std::out_of_range& ex) {
			throw wstring{ L"Переменная " + (*val).getWStr() + L" не определена\n" };
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
			throw wstring{ L"Переменная " + (*val).getWStr() + L" уже определена\n" };
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
			throw wstring{ L"Переменная " + (*val).getWStr() + L" не определена\n" };
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
			throw wstring{ L"Метка " + (*val).getWStr() + L" уже определена\n" };
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// checkParameterCount
// Проверяет соответствие количества инструкций
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void checkParameterCount(unsigned char type, int count, Machine* m, wstring* name, int stricted, int min, int range[], int variant[]) {
	switch (type) {
	case STRICTED:
		if (count != stricted) {
			throw wstring{ L"Инструкция " + *name + L" принимает " + to_wstring(stricted) + L" параметр(а)(ов)\n" };
		}
		break;
	case MIN:
		if (count < min) {
			throw wstring{ L"Инструкция " + *name + L" принимает не меньше " + to_wstring(min) + L" параметра(ов)\n" };
		}
		break;
	case RANGE:
		if (count < range[0] || count > range[1]) {
			throw wstring{ L"Инструкция " + *name + L" принимает от " + to_wstring(range[0]) + L" до " + to_wstring(range[1]) + L" параметра(ов)\n" };
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
			throw wstring{ L"Инструкция " + *name + L" может принимать следующее число параметров: " + params_str + L"\n" };
		}
	}
				 break;
	default:
		throw wstring{ L"Указан неверный вариант валидации параметров" };
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// requiredVar
// Проверяет то, что требуется переменная
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void requiredVar(Var* val, Machine* m, wstring* type, wstring num) {
	if ((*val).type != STR || (*val).getWStr()[0] != L'$') {
		throw wstring{ num + L" параметр инструкции " + (*type) + L" должен быть именем переменной\n" };
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// requiredLabel
// Проверяет то, что требуется метка
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void requiredLabel(Var* val, Machine* m, wstring* type, wstring num) {
	if ((*val).type != STR || (*val).getWStr()[0] != L'&') {
		throw wstring{ num + L" параметр инструкции " + (*type) + L" должен быть именем метки\n" };
	}
}
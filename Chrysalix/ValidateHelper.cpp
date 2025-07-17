#pragma once
#include <vector>
#include <map>
#include <string>
#include <iostream>

#include "ValidateHelper.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// getValue 
// ���������� �������� ��������� �� ����� ���������� ��� ��������
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Var getValue(Var* val, map<wstring, Var>* heap) {
	if ((*val).type == STR && (*val).getWStr()[0] == L'$') {
		try {
			return (*heap).at((*val).getWStr());
		}
		catch (std::out_of_range& ex) {
			throw wstring{ L"���������� " + (*val).getWStr() + L" �� ����������\n" };
		}
	}
	else {
		return *val;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// getLabel
// ���������� �������� ��������� �� ����� ���������� ��� ��������
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Var getLabel(Var* val, map<wstring, int>* pointers) {
	if ((*val).type == STR && (*val).getWStr()[0] == L'&') {
		try {
			return (*pointers).at((*val).getWStr());
		}
		catch (std::out_of_range& ex) {
			throw wstring{ L"����� " + (*val).getWStr() + L" �� ����������\n" };
		}
	}
	else {
		return *val;
	}
	return 0;
}


//������� ��������� ����������

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// checkExistValue
// ��������� ������������� ����������
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void checkExistValue(Var* val, Machine* m) {
	if ((*val).type == STR && (*val).getWStr()[0] == L'$') {
		if ((*m).heap.find((*val).getWStr()) != (*m).heap.end()) {
			throw wstring{ L"���������� " + (*val).getWStr() + L" ��� ����������\n" };
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// checkNotExistValue
// ��������� ���������� ������������� ����������
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void checkNotExistValue(Var* val, Machine* m) {
	if ((*val).type == STR && (*val).getWStr()[0] == L'$') {
		if ((*m).heap.find((*val).getWStr()) == (*m).heap.end()) {
			throw wstring{ L"���������� " + (*val).getWStr() + L" �� ����������\n" };
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// checkExistLabel
// ��������� ������������� �����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void checkExistLabel(Var* val, Machine* m) {
	if ((*val).type == STR && (*val).getWStr()[0] == L'&') {
		if ((*m).jmp_pointers.find((*val).getWStr()) != (*m).jmp_pointers.end()) {
			throw wstring{ L"����� " + (*val).getWStr() + L" ��� ����������\n" };
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// checkParameterCount
// ��������� ������������ ���������� ����������
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void checkParameterCount(unsigned char type, int count, Machine* m, wstring* name, int stricted, int min, int range[], int variant[]) {
	switch (type) {
	case STRICTED:
		if (count != stricted) {
			throw wstring{ L"���������� " + *name + L" ��������� " + to_wstring(stricted) + L" ��������(�)(��)\n" };
		}
		break;
	case MIN:
		if (count < min) {
			throw wstring{ L"���������� " + *name + L" ��������� �� ������ " + to_wstring(min) + L" ���������(��)\n" };
		}
		break;
	case RANGE:
		if (count < range[0] || count > range[1]) {
			throw wstring{ L"���������� " + *name + L" ��������� �� " + to_wstring(range[0]) + L" �� " + to_wstring(range[1]) + L" ���������(��)\n" };
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
			throw wstring{ L"���������� " + *name + L" ����� ��������� ��������� ����� ����������: " + params_str + L"\n" };
		}
	}
				 break;
	default:
		throw wstring{ L"������ �������� ������� ��������� ����������" };
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// requiredVar
// ��������� ��, ��� ��������� ����������
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void requiredVar(Var* val, Machine* m, wstring* type, wstring num) {
	if ((*val).type != STR || (*val).getWStr()[0] != L'$') {
		throw wstring{ num + L" �������� ���������� " + (*type) + L" ������ ���� ������ ����������\n" };
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// requiredLabel
// ��������� ��, ��� ��������� �����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void requiredLabel(Var* val, Machine* m, wstring* type, wstring num) {
	if ((*val).type != STR || (*val).getWStr()[0] != L'&') {
		throw wstring{ num + L" �������� ���������� " + (*type) + L" ������ ���� ������ �����\n" };
	}
}
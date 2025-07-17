#pragma once

#include <iostream>

#include "Helpers.h"

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
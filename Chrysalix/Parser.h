#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>

#include "Var.h"
#include "Machine.h"

using namespace std;

class Lexeme {
public:
	wstring type;
	vector<wstring> str_parameters;
	vector<Var> parameters;
};

class Parser {
public:
	wstring file;
	wstring file_content;
	
	Parser(wstring file_name);
	void fileLoad();
	void parse(Machine& m);

};

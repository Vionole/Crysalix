#pragma once

#include "Var.h"
#include "Machine.h"

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

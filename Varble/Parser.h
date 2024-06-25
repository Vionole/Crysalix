#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "VarClass.h"
#include "Machine.h"

using namespace std;

class Lexeme {
public:
	wstring type;
	wstring param_in_str;
	vector<Var> parameters;
};

class Parser {
public:
	wstring file;
	wstring file_content;
	Var refined;
	Var splitted;
	vector<vector<Var>> splitted_second;
	
	Parser(wstring file_name);
	void fileLoad();
	void commentRemover();
	void parse(Machine& m);

};

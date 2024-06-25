#pragma once
#include <sstream>
#include <fstream>
#include <codecvt>
#include <fcntl.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "Parser.h"
#include "VarClass.h"
#include "Machine.h"

using namespace std;

Parser::Parser(wstring file_name) {
	this->file = file_name;
}

void Parser::fileLoad() {
    //Чтение файла в кодировке UTF-8
    std::wifstream infile(this->file);
    if (infile) {
        infile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
        std::wstringstream wss;
        wss << infile.rdbuf();
        this->file_content = wss.str();
    }
    else {
        throw wstring{ L"Файл " + this->file + L" не найден\n" };
    }
    infile.close();
}

void Parser::commentRemover() {
    wstring str = L"";
    bool comment_char = false;
    for (auto& c : this->file_content) {
        if (!comment_char) {
            if (c != L'#' && c != L'\n' && c != L'\t') {
                str += c;
            } 
            else if (c == L'\n' || c == L'\t') {}
            else {
                comment_char = true;
            }
        }
        else {
            if (c == L'\n') {
                comment_char = false;
            }
        }
    }
    this->refined = str;
}

void Parser::parse(Machine& m) {
    this->commentRemover();
    this->splitted = this->refined.split(L";");
    vector<Var> split = this->splitted.getArr();
    vector<vector<Var>> splitTypeNArguments;
    for (auto& command : split) {
        Var cs = command.split(L":");
        splitTypeNArguments.push_back(cs.getArr());
    }
    
    vector<Lexeme> lxms;
    this->splitted_second = splitTypeNArguments;
    for (auto& command : this->splitted_second) {
        bool type = true;
        Lexeme lex;
        for (auto& lexemes : command) {
            lexemes = lexemes.trim();
            if (type) {
                lex.type = lexemes.getWStr();
                type = false;
            } 
            else {
                lex.param_in_str = lexemes.getWStr();
                lxms.push_back(lex);
                type = true;
            }
        }
    }

    vector<Lexeme> lxms_array;
    for (auto& lexeme : lxms) {
        Lexeme lex;
        lex.type = lexeme.type;
        Var str(lexeme.param_in_str);
        Var vec = str.split(L",");
        lex.parameters = vec.getArr();
        lxms_array.push_back(lex);
        
    }

    for (int i = 0; i < lxms_array.size(); ++i) {
        for (int j = 0; j < lxms_array[i].parameters.size(); ++j) {
            lxms_array[i].parameters[j] = lxms_array[i].parameters[j].trim();
        }
    }

    for (int i = 0; i < lxms_array.size(); ++i) {
        for (int j = 0; j < lxms_array[i].parameters.size(); ++j) {
            if (lxms_array[i].parameters[j].slice(0, 1).getWStr() == L"$" || lxms_array[i].parameters[j].slice(0, 1).getWStr() == L"&") {
                lxms_array[i].parameters[j] = lxms_array[i].parameters[j].toSTR();
            }
            else if (lxms_array[i].parameters[j].slice(0, 3).getWStr() == L"NTG") {
                wstring str = lxms_array[i].parameters[j].getWStr();
                str.erase(0, 3);
                Var v(str);
                lxms_array[i].parameters[j] = v.toNTG();
            }
            else if (lxms_array[i].parameters[j].slice(0, 4).getWStr() == L"UNTG") {
                wstring str = lxms_array[i].parameters[j].getWStr();
                str.erase(0, 4);
                Var v(str);
                lxms_array[i].parameters[j] = v.toUNTG();
            }
            else if (lxms_array[i].parameters[j].slice(0, 3).getWStr() == L"DBL") {
                wstring str = lxms_array[i].parameters[j].getWStr();
                str.erase(0, 3);
                Var v(str);
                lxms_array[i].parameters[j] = v.toDBL();
            }
            else if (lxms_array[i].parameters[j].slice(0, 3).getWStr() == L"CHR") {
                wstring str = lxms_array[i].parameters[j].getWStr();
                str.erase(0, 3);
                Var v(str);
                lxms_array[i].parameters[j] = v.toCHR();
            }
            else if (lxms_array[i].parameters[j].slice(0, 4).getWStr() == L"UCHR") {
                wstring str = lxms_array[i].parameters[j].getWStr();
                str.erase(0, 4);
                Var v(str);
                lxms_array[i].parameters[j] = v.toUCHR();
            }
            else if (lxms_array[i].parameters[j].slice(0, 4).getWStr() == L"TRUE") {

                lxms_array[i].parameters[j] = Var(true);
            }
            else if (lxms_array[i].parameters[j].slice(0, 5).getWStr() == L"FALSE") {

                lxms_array[i].parameters[j] = Var(false);
            }
            else if (lxms_array[i].parameters[j].slice(0, 3).getWStr() == L"NIL") {
                lxms_array[i].parameters[j] = Var();
            }
            else if (lxms_array[i].parameters[j].slice(0, 1).getWStr() == L"'") {
                wstring str = lxms_array[i].parameters[j].getWStr();
                str.erase(0, 1);
                str.pop_back();
                lxms_array[i].parameters[j] = Var(str);
                lxms_array[i].parameters[j] = lxms_array[i].parameters[j].repl(L"[COLON]", L":");
                lxms_array[i].parameters[j] = lxms_array[i].parameters[j].repl(L"[SEMICOLON]", L";");
                lxms_array[i].parameters[j] = lxms_array[i].parameters[j].repl(L"[COMMA]", L",");
                lxms_array[i].parameters[j] = lxms_array[i].parameters[j].repl(L"[QUOTE]", L"'");
                lxms_array[i].parameters[j] = lxms_array[i].parameters[j].repl(L"[TAB]", L"\t");
                lxms_array[i].parameters[j] = lxms_array[i].parameters[j].repl(L"[ENDL]", L"\n");
            }
        }
    }

    for (auto& lexeme : lxms_array) {
        if (lexeme.type == L"NOP") {
            m.instructions.push_back(new InstructNOP(lexeme.parameters));
        }
        else if (lexeme.type == L"END") {
            m.instructions.push_back(new InstructEND(lexeme.parameters));
        }
        else if (lexeme.type == L"VAR") {
            m.instructions.push_back(new InstructVAR(lexeme.parameters));
        }
        else if (lexeme.type == L"PRINT") {
            m.instructions.push_back(new InstructPRINT(lexeme.parameters));
        }
        else if (lexeme.type == L"FREE") {
            m.instructions.push_back(new InstructFREE(lexeme.parameters));
        }
        else if (lexeme.type == L"LBL") {
            m.instructions.push_back(new InstructLBL(lexeme.parameters));
        }
        else if (lexeme.type == L"JMP") {
            m.instructions.push_back(new InstructJMP(lexeme.parameters));
        }
        else
        {
            throw wstring{ lexeme.type + L": Неизвестная инструкция\n" };
        }
    }
}
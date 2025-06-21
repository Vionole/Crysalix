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
    //������ ����� � ��������� UTF-8
    std::wifstream infile(this->file);
    if (infile) {
        infile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
        std::wstringstream wss;
        wss << infile.rdbuf();
        this->file_content = wss.str();
    }
    else {
        throw wstring{ L"���� " + this->file + L" �� ������\n" };
    }
    infile.close();
}

void Parser::parse(Machine& m) {
    //���������� ������ ������ � ������
    wstring str = L"";
    bool instruction_parameters = false;
    bool first_comment_char = false;
    bool is_long_comment = false;
    bool is_comment = false;
    bool is_string = false;
    bool escape = false;
    Lexeme instruction;
    vector<Lexeme> lexemes;

    for (wchar_t& c : this->file_content) {
        //�������� �����������, ���� ��� ����, ���������� ������ ����� ������ ����������� �� ����� ������
        if (is_comment) {
            //���� ���������� ���� ��� ����� #
            if (first_comment_char == true) {
                //���� � ��� ������������� �����������
                if (is_long_comment == true) {
                    //� ���������� ���� #, ������������� ����������� �������������
                    if (c == L'#') {
                        is_long_comment = false;
                        is_comment = false;
                    }
                }
                else {
                    //���� ���������� ���� ��� # � ������������� ����������� �� �������, �������� ������������� �����������
                    if (c == L'#') {
                        is_long_comment = true;
                    }
                }
                //� ����� ������ ���������� ���� ������� �������
                first_comment_char = false;
            }
            else {
                //���� ���������� ����  �� ��� # � � ��� ������� �����������
                if (is_long_comment == true) {
                    //���� ������� ���� # ������ ���� ������� ����� �����������
                    if (c == L'#') {
                        first_comment_char = true;
                    }
                }
                //���� ��� ������������ �����������, ����������� ��� � ������� �������� �� ������ ������
                else {
                    if (c == L'\n') {
                        is_comment = false;
                        is_long_comment = false;
                        first_comment_char = false;
                    }
                }
            }

        }
        else {
            //���� � ��� ������� ' ��� ������, �������� ��� ��������� ������. ���������� ��� �������������� �������
            if (c == L'\'') {
                if (is_string == true) {
                    if (escape == false) {
                        is_string = false;
                    }
                }
                else {
                    is_string = true;
                }
                escape = false;
                str += c;
            }
            else {
                if (is_string == true) {
                    if (c == L'\\') {
                        if (escape == false) {
                            escape = true;
                        }
                        else {
                            escape = false;
                        }
                    }
                    else {
                        escape = false;
                    }
                    str += c;
                }
                else {
                    //���� � ��� ������� ���� #, � ��� ������� ������������ ��� ������������� �����������
                    if (c == L'#') {
                        if (is_comment == false) {
                            is_comment = true;
                            first_comment_char = true;
                        }
                    }
                    else {
                        //���������� ������� �� ��� ���, ���� �� ������ ���� :
                        //��� ����� ������������ ����������
                        if (instruction_parameters == false) {
                            if (c == L':') {
                                str.erase(0, str.find_first_not_of(L" \n\r\t"));
                                str.erase(str.find_last_not_of(L" \n\r\t") + 1);
                                instruction.type = str;
                                str = L"";
                                //������������� ����, ��� �������� ��������� ����������
                                instruction_parameters = true;
                            }
                            else {
                                //����� ���������� ������ � ���������� � ��������� ����������
                                str += c;
                            }
                        }
                        else {
                            //������ ��������� ����������. ���� � ��� �������, �������� ��������.
                            if (c == ',') {
                                str.erase(0, str.find_first_not_of(L" \n\r\t"));
                                str.erase(str.find_last_not_of(L" \n\r\t") + 1);
                                if (str != L"") {
                                    instruction.str_parameters.push_back(str);
                                    str = L"";
                                }
                            }
                            //���� ����� � �������, ���������� ������ ���������
                            else if (c == ';') {
                                str.erase(0, str.find_first_not_of(L" \n\r\t"));
                                str.erase(str.find_last_not_of(L" \n\r\t") + 1);
                                if (str != L"") {
                                    instruction.str_parameters.push_back(str);
                                    str = L"";
                                }
                                lexemes.push_back(instruction);
                                instruction.str_parameters.clear();
                                instruction.type = L"";
                                instruction_parameters = false;
                            }
                            else {
                                str += c;
                            }
                        }
                    }
                }
            }
        }
    }


    //������ ���������, ��������� �������� � ������ VAR �� ���������
    int size = lexemes.size();
    for (int i = 0; i < size; ++i) {
        try {
            int params_size = lexemes[i].str_parameters.size();
            for (int j = 0; j < params_size; ++j) {
                if (lexemes[i].str_parameters[j][0] == L'$' || lexemes[i].str_parameters[j][0] == L'&') {
                    lexemes[i].parameters.push_back(Var(lexemes[i].str_parameters[j]));
                }
                else if (lexemes[i].str_parameters[j].substr(0, 3) == L"NTG" || lexemes[i].str_parameters[j].substr(0, 3) == L"ntg") {
                    lexemes[i].parameters.push_back(Var(lexemes[i].str_parameters[j].erase(0, 3)).toNTG());
                }
                else if (lexemes[i].str_parameters[j].substr(0, 4) == L"UNTG" || lexemes[i].str_parameters[j].substr(0, 4) == L"untg") {
                    lexemes[i].parameters.push_back(Var(lexemes[i].str_parameters[j].erase(0, 4)).toUNTG());
                }
                else if (lexemes[i].str_parameters[j].substr(0, 3) == L"DBL" || lexemes[i].str_parameters[j].substr(0, 3) == L"dbl") {
                    lexemes[i].parameters.push_back(Var(lexemes[i].str_parameters[j].erase(0, 3)).toDBL());
                }
                else if (lexemes[i].str_parameters[j].substr(0, 3) == L"CHR" || lexemes[i].str_parameters[j].substr(0, 3) == L"chr") {
                    lexemes[i].parameters.push_back(Var(lexemes[i].str_parameters[j].erase(0, 3)).toCHR());
                }
                else if (lexemes[i].str_parameters[j].substr(0, 4) == L"UCHR" || lexemes[i].str_parameters[j].substr(0, 4) == L"uchr") {
                    lexemes[i].parameters.push_back(Var(lexemes[i].str_parameters[j].erase(0, 4)).toUCHR());
                }
                else if (lexemes[i].str_parameters[j] == L"TRUE" || lexemes[i].str_parameters[j] == L"true") {
                    lexemes[i].parameters.push_back(Var(true));
                }
                else if (lexemes[i].str_parameters[j] == L"FALSE" || lexemes[i].str_parameters[j] == L"false") {
                    lexemes[i].parameters.push_back(Var(false));
                }
                else if (lexemes[i].str_parameters[j] == L"NIL" || lexemes[i].str_parameters[j] == L"nil") {
                    lexemes[i].parameters.push_back(Var());
                }
                else if (lexemes[i].str_parameters[j][0] == L'\'') {
                    wstring str = lexemes[i].str_parameters[j].erase(0, lexemes[i].str_parameters[j].find_first_not_of(L"\'"));
                    str = lexemes[i].str_parameters[j].erase(lexemes[i].str_parameters[j].find_last_not_of(L"\'") + 1);
                   
                    wstring new_str = L"";
                    bool escape_ch = false;
                    for (wchar_t& c : str) {
                        if (c != L'\\' && !escape_ch) {
                            new_str += c;
                            continue;
                        }
                        if (c == L'\\' && !escape_ch) {
                            escape_ch = true;
                            continue;
                        }
                        if (escape_ch) {
                            switch (c) {
                            case L't':
                                new_str += L'\t';
                                break;
                            case L'n':
                                new_str += L'\n';
                                break;
                            case L'a':
                                new_str += L'\a';
                                break;
                            case L'q':
                                new_str += L'\'';
                                break;
                            case L'b':
                                new_str += L'\b';
                                break;
                            case L'f':
                                new_str += L'\f';
                                break;
                            case L'r':
                                new_str += L'\r';
                                break;
                            case L'v':
                                new_str += L'\v';
                                break;
                            case L'"':
                                new_str += L'"';
                                break;
                            case L'?':
                                new_str += L'?';
                                break;
                            case L'\'':
                                new_str += L'\'';
                                break;
                            case L'\\':
                                new_str += L'\\';
                            }
                            escape_ch = false;
                            continue;
                        }
                    }
                    lexemes[i].parameters.push_back(Var(new_str));
                }
                else {
                    throw wstring{lexemes[i].str_parameters[j] + L": ����������� �������"};

                }
            }
        }
        catch (const std::wstring& error_message) {
            throw wstring{ L"�������������� ������ � ���������� " + to_wstring(i + 1) + L": " + error_message };
        }
    }
    
    int i = 1;
    for (Lexeme& lexeme : lexemes) {
        if (lexeme.type == L"NOP" || lexeme.type == L"nop") {
            m.instructions.push_back(new InstructNOP(&lexeme.parameters));
        }
        else if (lexeme.type == L"END" || lexeme.type == L"end") {
            m.instructions.push_back(new InstructEND(&lexeme.parameters));
        }
        else if (lexeme.type == L"PAUSE" || lexeme.type == L"pause") {
            m.instructions.push_back(new InstructPAUSE(&lexeme.parameters));
        }
        else if (lexeme.type == L"SLEEP" || lexeme.type == L"sleep") {
            m.instructions.push_back(new InstructSLEEP(&lexeme.parameters));
        }
        else if (lexeme.type == L"VARIABLE" || lexeme.type == L"variable") {
            m.instructions.push_back(new InstructVARIABLE(&lexeme.parameters));
        }
        else if (lexeme.type == L"PRINT" || lexeme.type == L"print") {
            m.instructions.push_back(new InstructPRINT(&lexeme.parameters));
        }
        else if (lexeme.type == L"FREE" || lexeme.type == L"free") {
            m.instructions.push_back(new InstructFREE(&lexeme.parameters));
        }
        else if (lexeme.type == L"LABEL" || lexeme.type == L"label") {
            m.instructions.push_back(new InstructLABEL(&lexeme.parameters));
        }
        else if (lexeme.type == L"JUMP" || lexeme.type == L"jump") {
            m.instructions.push_back(new InstructJUMP(&lexeme.parameters));
        }
        else if (lexeme.type == L"INPUT" || lexeme.type == L"input") {
            m.instructions.push_back(new InstructINPUT(&lexeme.parameters));
        }
        else if (lexeme.type == L"CHANGE" || lexeme.type == L"change") {
            m.instructions.push_back(new InstructCHANGE(&lexeme.parameters));
        }
        else if (lexeme.type == L"TO" || lexeme.type == L"to") {
            m.instructions.push_back(new InstructTO(&lexeme.parameters));
        }
        else if (lexeme.type == L"CALC" || lexeme.type == L"calc") {
            m.instructions.push_back(new InstructCALC(&lexeme.parameters));
        }
        else
        {
            throw wstring{ L"�������������� ������ � ���������� " + to_wstring(i) + L": " + lexeme.type + L": ����������� ����������\n" };
        }
        ++i;
    }
}
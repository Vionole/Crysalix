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
    bool first_comment_char = false;
    bool multistring_comment = false;
    bool is_string = false;
    for (auto& c : this->file_content) {
        if (is_string) {
            if (c == L'\'') {
                is_string = false;
            }
            str += c;
            continue;
        }

        if (!comment_char) {
            if (c == L'\'') {
                is_string = true;
            }
            if (c != L'#' && c != L'\n' && c != L'\t') {
                str += c;
            } 
            else if (c == L'\n' || c == L'\t') {}
            else {
                comment_char = true;
                first_comment_char = true;
            }
        }
        else {
            if (!multistring_comment) {
                if (c == L'\n') {
                    comment_char = false;
                }
                else if (c == L'#' && first_comment_char) {
                    multistring_comment = true;
                }
                first_comment_char = false;
            }
            else {
                if (c == L'#' && !first_comment_char) {
                    first_comment_char = true;
                }
                else if (c == L'#' && first_comment_char) {
                    multistring_comment = false;
                    comment_char = false;
                    first_comment_char == false;
                }
                else {
                    first_comment_char = false;
                }
            }
        }
    }
    this->refined = str;
}

void Parser::parse(Machine& m) {

    //Убираем комментарии
    this->commentRemover();

    //Разрезаем на инструкции
    bool is_string = false;
    vector<Var> instructions;
    wstring str = L"";
    for (auto& c : this->refined.getWStr()) {
        if (is_string) {
            str += c;
            if (c == L'\'') {
                is_string = false;
            }
        }
        else {
            if (c == ';') {
                instructions.push_back(Var(str));
                str = L"";
            }
            else if (c == L'\'') {
                is_string = true;
                str += c;
            }
            else {
                str += c;
            }
        }
    }

    //Записываем "хвост, если он есть, благодаря незакрытой кавычке"
    if (str != L"") {
        instructions.push_back(Var(str));
    }

    //Заполняем массив инструкций
    this->splitted = Var(instructions);

    //Разрезаем инструкции на тип и аргумент
    vector<Var> split = this->splitted.getArr();
    vector<vector<Var>> splitTypeNArguments;
    for (auto& command : split) {
        wstring instr = L"";
        wstring params = L"";
        bool is_colon = false;
        for (auto& c : command.getWStr()) {
            if (is_colon) {
                params += c;
            }
            else {
                if (c != L':') {
                    instr += c;
                }
                else {
                    is_colon = true;
                }
            }
        }
        vector<Var> splitted_command;
        splitted_command.push_back(Var(instr));
        splitted_command.push_back(Var(params));
        splitTypeNArguments.push_back(splitted_command);
    }
    
    //Обрезаем пробелы у лексем
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

   //Разделяем строку параметров на отдельные параметры
    vector<Lexeme> lxms_array;
    for (auto& lexeme : lxms) {
        Lexeme lex;
        lex.type = lexeme.type;
        vector<Var>vec;

        wstring str = L"";
        bool is_string = false;
        for (auto& c : lexeme.param_in_str) {
            if (is_string) {
                str += c;
                if (c == L'\'') {
                    is_string = false;
                }
            }
            else {
                if (c == L'\'') {
                    is_string = true;
                }
                if (c == L',') {
                    vec.push_back(Var(str));
                    str = L"";
                    continue;
                }
                str += c;
            }
        }

        //Если запятой нет, это последний параметр, вносим в массив
        if (str != L"") {
            vec.push_back(Var(str));
        }

        lex.parameters = vec;
        lxms_array.push_back(lex);
        
    }

    //Обрезаем пробелы у отдельных параметров
    for (int i = 0; i < lxms_array.size(); ++i) {
        for (int j = 0; j < lxms_array[i].parameters.size(); ++j) {
            lxms_array[i].parameters[j] = lxms_array[i].parameters[j].trim();
        }
    }

    //Парсим параметры, превращая литералы в объект VAR со значением
    for (int i = 0; i < lxms_array.size(); ++i) {
        try {
            for (int j = 0; j < lxms_array[i].parameters.size(); ++j) {
                if (lxms_array[i].parameters[j].slice(0, 1).getWStr() == L"$" || lxms_array[i].parameters[j].slice(0, 1).getWStr() == L"&") {
                    lxms_array[i].parameters[j] = lxms_array[i].parameters[j].toSTR();
                }
                else if (lxms_array[i].parameters[j].slice(0, 3).getWStr() == L"NTG" || lxms_array[i].parameters[j].slice(0, 3).getWStr() == L"ntg") {
                    lxms_array[i].parameters[j] = (Var(lxms_array[i].parameters[j].getWStr().erase(0, 3))).toNTG();
                }
                else if (lxms_array[i].parameters[j].slice(0, 4).getWStr() == L"UNTG" || lxms_array[i].parameters[j].slice(0, 4).getWStr() == L"untg") {
                    lxms_array[i].parameters[j] = (Var(lxms_array[i].parameters[j].getWStr().erase(0, 4))).toUNTG();
                }
                else if (lxms_array[i].parameters[j].slice(0, 3).getWStr() == L"DBL" || lxms_array[i].parameters[j].slice(0, 3).getWStr() == L"dbl") {
                    lxms_array[i].parameters[j] = (Var(lxms_array[i].parameters[j].getWStr().erase(0, 3))).toDBL();
                }
                else if (lxms_array[i].parameters[j].slice(0, 3).getWStr() == L"CHR" || lxms_array[i].parameters[j].slice(0, 3).getWStr() == L"chr") {
                    lxms_array[i].parameters[j] = (Var(lxms_array[i].parameters[j].getWStr().erase(0, 3))).toCHR();
                }
                else if (lxms_array[i].parameters[j].slice(0, 4).getWStr() == L"UCHR" || lxms_array[i].parameters[j].slice(0, 4).getWStr() == L"uchr") {
                    lxms_array[i].parameters[j] = (Var(lxms_array[i].parameters[j].getWStr().erase(0, 4))).toUCHR();
                }
                else if (lxms_array[i].parameters[j].slice(0, 4).getWStr() == L"TRUE" || lxms_array[i].parameters[j].slice(0, 4).getWStr() == L"true") {
                    lxms_array[i].parameters[j] = Var(true);
                }
                else if (lxms_array[i].parameters[j].slice(0, 5).getWStr() == L"FALSE" || lxms_array[i].parameters[j].slice(0, 5).getWStr() == L"false") {
                    lxms_array[i].parameters[j] = Var(false);
                }
                else if (lxms_array[i].parameters[j].slice(0, 3).getWStr() == L"NIL" || lxms_array[i].parameters[j].slice(0, 3).getWStr() == L"nil") {
                    lxms_array[i].parameters[j] = Var();
                }
                else if (lxms_array[i].parameters[j].slice(0, 1).getWStr() == L"'") {
                    wstring str = lxms_array[i].parameters[j].getWStr().erase(0, 1);
                    str.pop_back();

                    wstring new_str = L"";
                    bool escape_ch = false;
                    for (auto& c : str) {
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
                            case L'\\':
                                new_str += L'\\';
                            }
                            escape_ch = false;
                            continue;
                        }
                    }

                    lxms_array[i].parameters[j] = Var(new_str);
                }
                else {
                    throw wstring{ lxms_array[i].parameters[j].getWStr() + L": Неизвестный параметр\n" };
                }
            }
        }
        catch (const std::wstring& error_message) {
            throw wstring{ L"Синтаксическая ошибка в инструкции " + to_wstring(i + 1) + L": " + error_message};
        }
    }

    int i = 1;
    for (auto& lexeme : lxms_array) {
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
        else if (lexeme.type == L"VAR" || lexeme.type == L"var") {
            m.instructions.push_back(new InstructVAR(&lexeme.parameters));
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
        else if (lexeme.type == L"JMP" || lexeme.type == L"jmp") {
            m.instructions.push_back(new InstructJMP(&lexeme.parameters));
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
        else
        {
            throw wstring{ L"Синтаксическая ошибка в инструкции " + to_wstring(i) + L": " + lexeme.type + L": Неизвестная инструкция\n" };
        }
        ++i;
    }
}
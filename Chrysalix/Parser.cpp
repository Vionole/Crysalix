﻿#pragma once

#include <sstream>
#include <fstream>
#include <codecvt>

#include "Parser.h"
#include "CTable.h"

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

void Parser::parse(Machine& m) {
    //Перебираем каждый символ в строке
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
        //Вырезаем комментарии, если они есть, игнорируем сиволы после начала комментария до конца строки
        if (is_comment) {
            //Если предыдущий знак был равен #
            if (first_comment_char == true) {
                //Если у нас многострочный комментарий
                if (is_long_comment == true) {
                    //и попадается знак #, многострочный комментарий заканчивается
                    if (c == L'#') {
                        is_long_comment = false;
                        is_comment = false;
                    }
                }
                else {
                    //Если предыдущий знак был # а многострочный комментарий не начался, начинаем многострочный комментарий
                    if (c == L'#') {
                        is_long_comment = true;
                    }
                }
                //В любом случае сбрасываем флаг первого символа
                first_comment_char = false;
            }
            else {
                //Если предыдущий знак  не был # и у нас длинный кооментарий
                if (is_long_comment == true) {
                    //Если текущий знак # ставим флаг первого знака комментария
                    if (c == L'#') {
                        first_comment_char = true;
                    }
                }
                //если это однострочный комментарий, заканчиваем его с момента перевода на другую строку
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
            //Если у нас кавычка ' это значит, началась или кончилась строка. Игнорируем все синтаксические символы
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
                    //Если у нас имеется знак #, у нас начался однострочный или многострочный комментарий
                    if (c == L'#') {
                        if (is_comment == false) {
                            is_comment = true;
                            first_comment_char = true;
                        }
                    }
                    else {
                        //Перебираем символы до тех пор, пока не найдем двоеточие :
                        //Это конец наименования инструкции
                        if (instruction_parameters == false) {
                            if (c == L':') {
                                str.erase(0, str.find_first_not_of(L" \n\r\t"));
                                str.erase(str.find_last_not_of(L" \n\r\t") + 1);
                                instruction.type = str;
                                str = L"";
                                //устанавливаем флаг, что начались параметры инструкции
                                instruction_parameters = true;
                            }
                            else {
                                //Иначе записываем символ в переменную с названием инструкции
                                str += c;
                            }
                        }
                        else {
                            //Парсим параметры инструкции. Если у нас запятая, параметр кончился.
                            if (c == ',') {
                                str.erase(0, str.find_first_not_of(L" \n\r\t"));
                                str.erase(str.find_last_not_of(L" \n\r\t") + 1);
                                if (str != L"") {
                                    instruction.str_parameters.push_back(str);
                                    str = L"";
                                }
                            }
                            //если точка с запятой, инструкция вообще кончилась
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


    //Парсим параметры, превращая литералы в объект VAR со значением
    int size = (int)lexemes.size();
    for (int i = 0; i < size; ++i) {
        try {
            int params_size = (int)lexemes[i].str_parameters.size();
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
                else if (lexemes[i].str_parameters[j] == L"ARR" || lexemes[i].str_parameters[j] == L"arr") {
                    lexemes[i].parameters.push_back(Var(vector<Var>()));
                }
                else if (lexemes[i].str_parameters[j] == L"MAP" || lexemes[i].str_parameters[j] == L"map") {
                    lexemes[i].parameters.push_back(Var(map<wstring, Var>()));
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
                    wstring temp = lexemes[i].str_parameters[j];
                    wstring str = temp.erase(0, temp.find_first_not_of(L"\'"));
                    str = temp.erase(temp.find_last_not_of(L"\'") + 1);
                   
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
                } //Пытаемся работать с числами, если не указан тип данных
                else if (lexemes[i].str_parameters[j][0] == L'0'
                    || lexemes[i].str_parameters[j][0] == L'1'
                    || lexemes[i].str_parameters[j][0] == L'2'
                    || lexemes[i].str_parameters[j][0] == L'3'
                    || lexemes[i].str_parameters[j][0] == L'4'
                    || lexemes[i].str_parameters[j][0] == L'5'
                    || lexemes[i].str_parameters[j][0] == L'6'
                    || lexemes[i].str_parameters[j][0] == L'7'
                    || lexemes[i].str_parameters[j][0] == L'8'
                    || lexemes[i].str_parameters[j][0] == L'9'
                    || lexemes[i].str_parameters[j][0] == L'+'
                    || lexemes[i].str_parameters[j][0] == L'-') {
                    Var numberdbl = Var(lexemes[i].str_parameters[j]).toDBL();
                    Var numberntg = Var(lexemes[i].str_parameters[j]).toNTG();
                    if (numberdbl == numberntg) {
                        lexemes[i].parameters.push_back(numberntg);
                    }
                    else {
                        lexemes[i].parameters.push_back(numberdbl);
                    }
                }
                else {
                    throw wstring{lexemes[i].str_parameters[j] + L": Неизвестный литерал"};

                }
            }
        }
        catch (const std::wstring& error_message) {
            throw wstring{ L"Синтаксическая ошибка в инструкции " + to_wstring(i + 1) + L": " + error_message };
        }
    }
    
    int i = 1;
    Instruction inst;

    //Таблица опкодов
    CTable table;

    for (Lexeme& lexeme : lexemes) {
        try {
            inst.opCode = table.opCodeMap.at(lexeme.type);
            inst.as_string = lexeme.type + L": ";
        }
        catch (std::out_of_range& ex) {
            throw wstring{ L"Синтаксическая ошибка в инструкции " + to_wstring(i) + L": " + lexeme.type + L": Неизвестная инструкция\n" };
        }

        inst.parameters = lexeme.parameters;
        int max_size = lexeme.str_parameters.size();
        for (int param = 0; param < max_size; ++param) {
            inst.as_string += lexeme.str_parameters[param];
            if (param != max_size - 1) {
                inst.as_string += L", ";
            }
        }
        inst.as_string += L";";
        m.instructions.push_back(inst);
        ++i;
    }
}
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
                } //Пытаемся равботать с числами, если не указан тип данных
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
    for (Lexeme& lexeme : lexemes) {
        if (lexeme.type == L"NOP" || lexeme.type == L"nop") {
            inst.opCode = NOP;
        }
        else if (lexeme.type == L"END" || lexeme.type == L"end") {
            inst.opCode = END;
        }
        else if (lexeme.type == L"PAUSE" || lexeme.type == L"pause") {
            inst.opCode = PAUSE;
        }
        else if (lexeme.type == L"SLEEP" || lexeme.type == L"sleep") {
            inst.opCode = SLEEP;
        }
        else if (lexeme.type == L"VAR" || lexeme.type == L"var") {
            inst.opCode = VAR;
        }
        else if (lexeme.type == L"PRINT" || lexeme.type == L"print") {
            inst.opCode = PRINT;
        }
        else if (lexeme.type == L"FREE" || lexeme.type == L"free") {
            inst.opCode = FREE;
        }
        else if (lexeme.type == L"LABEL" || lexeme.type == L"label") {
            inst.opCode = LABEL;
        }
        else if (lexeme.type == L"JUMP" || lexeme.type == L"jump") {
            inst.opCode = JUMP;
        }
        else if (lexeme.type == L"INPUT" || lexeme.type == L"input") {
            inst.opCode = INPT;
        }
        else if (lexeme.type == L"CHANGE" || lexeme.type == L"change") {
            inst.opCode = CHANGE;
        }
        else if (lexeme.type == L"TO" || lexeme.type == L"to") {
            inst.opCode = TO;
        }
        else if (lexeme.type == L"CALC" || lexeme.type == L"calc") {
            inst.opCode = CALC;
        }
        else if (lexeme.type == L"NEWTEMP" || lexeme.type == L"newtemp") {
            inst.opCode = NEWTEMP;
        }
        else if (lexeme.type == L"FORGET" || lexeme.type == L"forget") {
            inst.opCode = FORGET;
        }
        else if (lexeme.type == L"TCOUNT" || lexeme.type == L"tcount") {
            inst.opCode = TCOUNT;
        }
        else if (lexeme.type == L"ISSET" || lexeme.type == L"isset") {
            inst.opCode = ISSET;
        }
        else if (lexeme.type == L"TYPEOF" || lexeme.type == L"typeof") {
            inst.opCode = TYPEOF;
        }
        else if (lexeme.type == L"COMP" || lexeme.type == L"comp") {
            inst.opCode = COMP;
        }
        else if (lexeme.type == L"LOGIC" || lexeme.type == L"logic") {
            inst.opCode = LOGIC;
        }
        else if (lexeme.type == L"JIF" || lexeme.type == L"jif") {
            inst.opCode = JIF;
        }
        else if (lexeme.type == L"JIFNOT" || lexeme.type == L"jifnot") {
            inst.opCode = JIFNOT;
        }
        else if (lexeme.type == L"DLABEL" || lexeme.type == L"dlabel") {
            inst.opCode = DLABEL;
        }
        else if (lexeme.type == L"PUSHB" || lexeme.type == L"pushb") {
            inst.opCode = PUSHB;
        }
        else if (lexeme.type == L"POPB" || lexeme.type == L"popb") {
            inst.opCode = POPB;
        }
        else if (lexeme.type == L"PUSHF" || lexeme.type == L"pushf") {
            inst.opCode = PUSHF;
        }
        else if (lexeme.type == L"POPF" || lexeme.type == L"popf") {
            inst.opCode = POPF;
        }
        else if (lexeme.type == L"ERASE" || lexeme.type == L"erase") {
            inst.opCode = ERASE;
        }
        else if (lexeme.type == L"INSERT" || lexeme.type == L"insert") {
            inst.opCode = INSRT;
        }
        else
        {
            throw wstring{ L"Синтаксическая ошибка в инструкции " + to_wstring(i) + L": " + lexeme.type + L": Неизвестная инструкция\n" };
        }
        inst.parameters = lexeme.parameters;
        m.instructions.push_back(inst);
        ++i;
    }
}
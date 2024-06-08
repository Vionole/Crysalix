
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include "VarClass.h"

using namespace std;

Var::Var() {
    this->type = NIL;
}

Var::Var(unsigned long long int untg) {
    this->type = UNTG;
    this->data.untg = untg;
}

Var::Var(long long int ntg) {
    this->type = NTG;
    this->data.ntg = ntg;
}

Var::Var(long int ntg) {
    this->type = NTG;
    this->data.ntg = (long long int)ntg;
}

Var::Var(int ntg) {
    this->type = NTG;
    this->data.ntg = (long long int)ntg;
}

Var::Var(short int ntg) {
    this->type = NTG;
    this->data.ntg = (long long int)ntg;
}

Var::Var(unsigned short int ntg) {
    this->type = NTG;
    this->data.ntg = (long long int)ntg;
}

Var::Var(unsigned int ntg) {
    this->type = NTG;
    this->data.ntg = (long long int)ntg;
}

Var::Var(unsigned long int ntg) {
    this->type = NTG;
    this->data.ntg = (long long int)ntg;
}

Var::Var(long double dbl) {
    this->type = DBL;
    this->data.dbl = dbl;
}

Var::Var(double dbl) {
    this->type = DBL;
    this->data.dbl = (long double)dbl;
}

Var::Var(float dbl) {
    this->type = DBL;
    this->data.dbl = (long double)dbl;
}

Var::Var(char chr) {
    this->type = CHR;
    this->data.chr = chr;
}

Var::Var(unsigned char uchr) {
    this->type = UCHR;
    this->data.uchr = uchr;
}

Var::Var(bool bln) {
    this->type = BLN;
    this->data.bln = bln;
}

Var::Var(wstring str) {
    this->type = STR;
    this->data.str = str;
}

Var::Var(const wchar_t* str) {
    this->type = STR;
    this->data.str = str;
}

Var::Var(wstring t, int i) {
    if (t == L"array") {
        this->type = ARR;
        this->arr = vector<Var>(i);
    } 
    else if (t == L"map" && i == 0) {
        this->type = MAP;
        this->mp = map<wstring, Var>();
    }
    else {
        this->type == UNKNOWN;
    }
}

long long int Var::getInt() {
    if (this->type == NTG) {
        return this->data.ntg;
    }
    else {
        throw wstring{ L"Переменная не является знаковой целочисленной" };
    }
}

unsigned long long int Var::getUInt() {
    if (this->type == UNTG) {
        return this->data.untg;
    }
    else {
        throw wstring{ L"Переменная не является беззнаковой целочисленной" };
    }
}

long double Var::getDouble() {
    if (this->type == DBL) {
        return this->data.dbl;
    }
    else {
        throw wstring{ L"Переменная не является числом с плавающей точкой" };
    }
}

char Var::getChar() {
    if (this->type == CHR) {
        return this->data.chr;
    }
    else {
        throw wstring{ L"Переменная не является символьной" };
    }
}

unsigned char Var::getUChar() {
    if (this->type == UCHR) {
        return this->data.uchr;
    }
    else {
        throw wstring{ L"Переменная не является беззнаковой символьной" };
    }
}

bool Var::getBool() {
    if (this->type == BLN) {
        return this->data.bln;
    }
    else {
        throw wstring{ L"Переменная не является булевой" };
    }
}

wstring Var::getWStr() {
    if (this->type == STR) {
        return this->data.str;
    }
    else {
        throw wstring{ L"Переменная не является строкой" };
    }
}

vector<Var> Var::getArr() {
    if (this->type == ARR) {
        return this->arr;
    }
    else {
        throw wstring{ L"Переменная не является массивом" };
    }
}

map<wstring, Var> Var::getMap() {
    if (this->type == MAP) {
        return this->mp;
    }
    else {
        throw wstring{ L"Переменная не является словарем" };
    }
}

Var Var::toNTG() {
    if (this->type == NTG) {
        Var result = this->data.ntg;
        return result;
    }
    else if (this->type == UNTG) {
        Var result;
        result.type = NTG;
        result.data.ntg = (long long int)this->data.untg;
        return result;
    }
    else if (this->type == DBL) {
        Var result;
        result.type = NTG;
        result.data.ntg = (long long int)this->data.dbl;
        return result;
    }
    else if (this->type == CHR) {
        Var result;
        result.type = NTG;
        result.data.ntg = (long long int)this->data.chr;
        return result;
    }
    else if (this->type == UCHR) {
        Var result;
        result.type = NTG;
        result.data.ntg = (long long int)this->data.uchr;
        return result;
    }
    else if (this->type == BLN) {
        Var result;
        result.type = NTG;
        result.data.ntg = (long long int)this->data.bln;
        return result;
    }
    else if (this->type == STR) {
        try {
            Var result;
            result.type = NTG;
            result.data.ntg = stoll(this->data.str);
            return result;
        }
        catch (exception& err)
        {
            throw wstring{ L"Не удалось привести строку к типу NTG" };
        }
    }
    else if (this->type == NIL) {
        Var result = 0;
        return result;
    }
    else if (this->type == UNKNOWN) {
        Var result = 0;
        return result;
    }
    else if (this->type == ARR) {
        throw wstring{ L"Невозможно привести массив к типу NTG" };
    }
    else if (this->type == MAP) {
        throw wstring{ L"Невозможно привести словарь к типу NTG" };
    }
}

Var Var::toUNTG() {
    if (this->type == NTG) {
        Var result;
        result.type = UNTG;
        result.data.untg = (unsigned long long int)this->data.ntg;
        return result;
    }
    else if (this->type == UNTG) {
        Var result;
        result.type = UNTG;
        result.data.untg = (unsigned long long int)this->data.untg;
        return result;
    }
    else if (this->type == DBL) {
        Var result;
        result.type = UNTG;
        result.data.untg = (unsigned long long int)this->data.dbl;
        return result;
    }
    else if (this->type == CHR) {
        Var result;
        result.type = UNTG;
        result.data.untg = (unsigned long long int)this->data.chr;
        return result;
    }
    else if (this->type == UCHR) {
        Var result;
        result.type = UNTG;
        result.data.untg = (unsigned long long int)this->data.uchr;
        return result;
    }
    else if (this->type == BLN) {
        Var result;
        result.type = UNTG;
        result.data.untg = (unsigned long long int)this->data.bln;
        return result;
    }
    else if (this->type == STR) {
        try {
            Var result;
            result.type = UNTG;
            result.data.untg = stoull(this->data.str);
            return result;
        }
        catch (exception& err)
        {
            throw wstring{ L"Не удалось привести строку к типу UNTG" };
        }
    }
    else if (this->type == NIL) {
        Var result = 0ULL;
        return result;
    }
    else if (this->type == UNKNOWN) {
        Var result = 0ULL;
        return result;
    }
    else if (this->type == ARR) {
        throw wstring{ L"Невозможно привести массив к типу UNTG" };
    }
    else if (this->type == MAP) {
        throw wstring{ L"Невозможно привести словарь к типу UNTG" };
    }
}

Var Var::toDBL() {
    if (this->type == NTG) {
        Var result;
        result.type = DBL;
        result.data.dbl = (long double)this->data.ntg;
        return result;
    }
    else if (this->type == UNTG) {
        Var result;
        result.type = DBL;
        result.data.dbl = (long double)this->data.untg;
        return result;
    }
    else if (this->type == DBL) {
        Var result;
        result.type = DBL;
        result.data.dbl = (long double)this->data.dbl;
        return result;
    }
    else if (this->type == CHR) {
        Var result;
        result.type = DBL;
        result.data.dbl = (long double)this->data.chr;
        return result;
    }
    else if (this->type == UCHR) {
        Var result;
        result.type = DBL;
        result.data.dbl = (long double)this->data.uchr;
        return result;
    }
    else if (this->type == BLN) {
        Var result;
        result.type = DBL;
        result.data.dbl = (long double)this->data.bln;
        return result;
    }
    else if (this->type == STR) {
        try {
            Var result;
            result.type = DBL;
            result.data.dbl = stold(this->data.str);
            return result;
        }
        catch (exception& err)
        {
            throw wstring{ L"Не удалось привести строку к типу DBL" };
        }
    }
    else if (this->type == NIL) {
        Var result = 0.0;
        return result;
    }
    else if (this->type == UNKNOWN) {
        Var result = 0.0;
        return result;
    }
    else if (this->type == ARR) {
        throw wstring{ L"Невозможно привести массив к типу DBL" };
    }
    else if (this->type == MAP) {
        throw wstring{ L"Невозможно привести словарь к типу DBL" };
    }
}

Var Var::toCHR() {
    if (this->type == NTG) {
        Var result;
        result.type = CHR;
        result.data.chr = (char)this->data.ntg;
        return result;
    }
    else if (this->type == UNTG) {
        Var result;
        result.type = CHR;
        result.data.chr = (char)this->data.untg;
        return result;
    }
    else if (this->type == DBL) {
        Var result;
        result.type = CHR;
        result.data.chr = (char)this->data.dbl;
        return result;
    }
    else if (this->type == CHR) {
        Var result;
        result.type = CHR;
        result.data.chr = (char)this->data.chr;
        return result;
    }
    else if (this->type == UCHR) {
        Var result;
        result.type = CHR;
        result.data.chr = (char)this->data.uchr;
        return result;
    }
    else if (this->type == BLN) {
        Var result;
        result.type = CHR;
        result.data.chr = (char)this->data.bln;
        return result;
    }
    else if (this->type == STR) {
        try {
            Var result;
            result.type = CHR;
            result.data.chr = stoll(this->data.str);
            return result;
        }
        catch (exception& err)
        {
            throw wstring{ L"Не удалось привести строку к типу CHR" };
        }
    }
    else if (this->type == NIL) {
        Var result = (char)0;
        return result;
    }
    else if (this->type == UNKNOWN) {
        Var result = (char)0;
        return result;
    }
    else if (this->type == ARR) {
        throw wstring{ L"Невозможно привести массив к типу CHR" };
    }
    else if (this->type == MAP) {
        throw wstring{ L"Невозможно привести словарь к типу CHR" };
    }
}

Var Var::toUCHR() {
    if (this->type == NTG) {
        Var result;
        result.type = UCHR;
        result.data.uchr = (unsigned char)this->data.ntg;
        return result;
    }
    else if (this->type == UNTG) {
        Var result;
        result.type = UCHR;
        result.data.uchr = (unsigned char)this->data.untg;
        return result;
    }
    else if (this->type == DBL) {
        Var result;
        result.type = UCHR;
        result.data.uchr = (unsigned char)this->data.dbl;
        return result;
    }
    else if (this->type == CHR) {
        Var result;
        result.type = UCHR;
        result.data.uchr = (unsigned char)this->data.chr;
        return result;
    }
    else if (this->type == UCHR) {
        Var result;
        result.type = UCHR;
        result.data.uchr = (unsigned char)this->data.uchr;
        return result;
    }
    else if (this->type == BLN) {
        Var result;
        result.type = UCHR;
        result.data.uchr = (unsigned char)this->data.bln;
        return result;
    }
    else if (this->type == STR) {
        try {
            Var result;
            result.type = UCHR;
            result.data.uchr = stoll(this->data.str);
            return result;
        }
        catch (exception& err)
        {
            throw wstring{ L"Не удалось привести строку к типу UCHR" };
        }
    }
    else if (this->type == NIL) {
        Var result = (unsigned char)0;
        return result;
    }
    else if (this->type == UNKNOWN) {
        Var result = (unsigned char)0;
        return result;
    }
    else if (this->type == ARR) {
        throw wstring{ L"Невозможно привести массив к типу UCHR" };
    }
    else if (this->type == MAP) {
        throw wstring{ L"Невозможно привести словарь к типу UCHR" };
    }
}

Var Var::toBLN() {
    if (this->type == NTG) {
        Var result;
        result.type = BLN;
        result.data.bln = (bool)this->data.ntg;
        return result;
    }
    else if (this->type == UNTG) {
        Var result;
        result.type = BLN;
        result.data.bln = (bool)this->data.untg;
        return result;
    }
    else if (this->type == DBL) {
        Var result;
        result.type = BLN;
        result.data.bln = (bool)this->data.dbl;
        return result;
    }
    else if (this->type == CHR) {
        Var result;
        result.type = BLN;
        result.data.bln = (bool)this->data.chr;
        return result;
    }
    else if (this->type == UCHR) {
        Var result;
        result.type = BLN;
        result.data.bln = (bool)this->data.uchr;
        return result;
    }
    else if (this->type == BLN) {
        Var result;
        result.type = BLN;
        result.data.bln = (bool)this->data.bln;
        return result;
    }
    else if (this->type == STR) {
        Var result;
        result.type = BLN;

        if (this->data.str == L"") {
            result.data.bln = false;
        }
        else {
            result.data.bln = true;
        }

        return result;
    }
    else if (this->type == NIL) {
        Var result = (bool)0;
        return result;
    }
    else if (this->type == UNKNOWN) {
        Var result = (bool)0;
        return result;
    }
    else if (this->type == ARR) {
        Var result;
        result.type = BLN;

        if (this->arr.empty()) {
            result.data.bln = false;
        }
        else {
            result.data.bln = true;
        }

        return result;
    }
    else if (this->type == MAP) {
        Var result;
        result.type = BLN;

        if (this->mp.empty()) {
            result.data.bln = false;
        }
        else {
            result.data.bln = true;
        }

        return result;
    }
}

Var Var::toSTR() {
    if (this->type == NTG) {
        Var result;
        result.type = STR;
        result.data.str = to_wstring(this->data.ntg);
        return result;
    }
    else if (this->type == UNTG) {
        Var result;
        result.type = STR;
        result.data.str = to_wstring(this->data.untg);
        return result;
    }
    else if (this->type == DBL) {
        Var result;
        result.type = STR;
        result.data.str = to_wstring(this->data.dbl);
        return result;
    }
    else if (this->type == CHR) {
        Var result;
        result.type = STR;
        result.data.str = to_wstring(this->data.chr);
        return result;
    }
    else if (this->type == UCHR) {
        Var result;
        result.type = STR;
        result.data.str = to_wstring(this->data.uchr);
        return result;
    }
    else if (this->type == BLN) {
        Var result;
        result.type = STR;

        if (this->data.bln) {
            result.data.str = L"true";
        }
        else {
            result.data.str = L"false";
        }

        return result;
    }
    else if (this->type == STR) {
        Var result;
        result.type = STR;
        result.data.str = this->data.str;
        return result;
    }
    else if (this->type == NIL) {
        Var result = L"";
        return result;
    }
    else if (this->type == UNKNOWN) {
        Var result = L"";
        return result;
    }
    else if (this->type == ARR) {
        throw wstring{ L"Невозможно привести массив к типу STR" };
    }
    else if (this->type == MAP) {
        throw wstring{ L"Невозможно привести словарь к типу STR" };
    }
}

Var Var::toARR() {
    if (this->type == NTG) {
        Var result;
        result.type = ARR;
        result.arr.push_back(*this);
        return result;
    }
    else if (this->type == UNTG) {
        Var result;
        result.type = ARR;
        result.arr.push_back(*this);
        return result;
    }
    else if (this->type == DBL) {
        Var result;
        result.type = ARR;
        result.arr.push_back(*this);
        return result;
    }
    else if (this->type == CHR) {
        Var result;
        result.type = ARR;
        result.arr.push_back(*this);
        return result;
    }
    else if (this->type == UCHR) {
        Var result;
        result.type = ARR;
        result.arr.push_back(*this);
        return result;
    }
    else if (this->type == BLN) {
        Var result;
        result.type = ARR;
        result.arr.push_back(*this);
        return result;
    }
    else if (this->type == STR) {
        Var result;
        result.type = ARR;
        result.arr.push_back(*this);
        return result;
    }
    else if (this->type == NIL) {
        Var result;
        result.type = ARR;
        result.arr.push_back(*this);
        return result;
    }
    else if (this->type == UNKNOWN) {
        Var result;
        result.type = ARR;
        result.arr.push_back(*this);
        return result;
    }
    else if (this->type == ARR) {
        return *this;
    }
    else if (this->type == MAP) {
        throw wstring{ L"Невозможно привести словарь к типу ARR" };
    }
}


void Var::print() {
    switch (this->type) {
    case UNTG:
        wcout << this->data.untg;
        break;
    case NTG:
        wcout << this->data.ntg;
        break;
    case DBL:
        wcout << this->data.dbl;
        break;
    case CHR:
        wcout << this->data.chr;
        break;
    case UCHR:
        wcout << this->data.uchr;
        break;
    case BLN:
        if (this->data.bln) {
            wcout << "true";
        }
        else {
            wcout << "false";
        }
        break;
    case STR:
        wcout << this->data.str;
        break;
    case NIL:
        wcout << L"NULL";
        break;
    case ARR:
        wcout << L"ARRAY";
        break;
    case MAP:
        wcout << L"MAP";
    default:
        wcout << L"UNKNOWN";
    }
}

wstring Var::typeOf() {
    wstring result = L"";
    switch (this->type) {
    case UNTG:
        result = L"UNTG";
        return result;
        break;
    case NTG:
        result = L"NTG";
        return result;
        break;
    case DBL:
        result = L"DBL";
        return result;
        break;
    case CHR:
        result = L"CHR";
        return result;
        break;
    case UCHR:
        result = L"UCHR";
        return result;
        break;
    case BLN:
        result = L"BLN";
        return result;
        break;
    case STR:
        result = L"STR";
        return result;
        break;
    case NIL:
        result = L"NIL";
        return result;
        break;
    case ARR:
        result = L"ARR";
        return result;
        break;
    case MAP:
        result = L"MAP";
        return result;
    default:
        result = L"UNKNOWN";
        return result;
    }
}

Var& Var::operator[](int ind) {
    if (this->type == ARR) {
        return this->arr.at(ind);
    }
    else if (this->type == STR) {
        wstring str(1, this->data.str.at(ind));
        Var *chr = new Var(str);
        return *chr;
    }
}

Var& Var::operator[](Var v) {
    if (this->type == ARR) {
        return this->arr.at(v.getInt());
    }
    else if (this->type == STR) {
        wstring str(1, this->data.str.at(v.getInt()));
        Var* chr = new Var(str);
        return *chr;
    }
}



wostream& operator<< (wostream& wos, const Var& var)
{
    switch (var.type) {
    case UNTG:
        return wos << var.data.untg;
        break;
    case NTG:
        return wos << var.data.ntg;
        break;
    case DBL:
        return wos << var.data.dbl;
        break;
    case CHR:
        return wos << var.data.chr;
        break;
    case UCHR:
        return wos << var.data.uchr;
        break;
    case BLN:
        if (var.data.bln) {
            return wos << L"true";
        }
        else {
            return wos << L"false";
        }
        break;
    case STR:
        return wos << var.data.str;
        break;
    case NIL:
        return wos << L"NIL";
        break;
    case ARR:
        return wos << L"ARRAY";
        break;
    case MAP:
        return wos << L"MAP";
    default:
        return wos << L"UNKNOWN";
    }
}

Var& Var::operator= (const Var& var) {
    this->type = var.type;
    this->data.untg = var.data.untg;
    this->data.ntg = var.data.ntg;
    this->data.dbl = var.data.dbl;
    this->data.chr = var.data.chr;
    this->data.uchr = var.data.uchr;
    this->data.bln = var.data.bln;
    this->data.str = var.data.str;
    this->arr = var.arr;
    this->mp = var.mp;
    return *this; 
}

Var& Var::operator= (const unsigned long long int& var) {
    this->type = UNTG;
    this->data.untg = var;
    return *this;
}

Var& Var::operator= (const unsigned long int& var) {
    this->type = NTG;
    this->data.ntg = var;
    return *this;
}

Var& Var::operator= (const unsigned int& var) {
    this->type = NTG;
    this->data.ntg = var;
    return *this;
}

Var& Var::operator= (const unsigned short int& var) {
    this->type = NTG;
    this->data.ntg = var;
    return *this;
}

Var& Var::operator= (const long long int& var) {
    this->type = NTG;
    this->data.ntg = var;
    return *this;
}

Var& Var::operator= (const long int& var) {
    this->type = NTG;
    this->data.ntg = var;
    return *this;
}

Var& Var::operator= (const int& var) {
    this->type = NTG;
    this->data.ntg = var;
    return *this;
}

Var& Var::operator= (const short int& var) {
    this->type = NTG;
    this->data.ntg = var;
    return *this;
}

Var& Var::operator= (const long double& var) {
    this->type = DBL;
    this->data.dbl = var;
    return *this;
}

Var& Var::operator= (const double& var) {
    this->type = DBL;
    this->data.dbl = (long double)var;
    return *this;
}

Var& Var::operator= (const float& var) {
    this->type = DBL;
    this->data.dbl = (const long double)var;
    return *this;
}

Var& Var::operator= (const char& var) {
    this->type = CHR;
    this->data.chr = var;
    return *this;
}

Var& Var::operator= (const unsigned char& var) {
    this->type = UCHR;
    this->data.uchr = var;
    return *this;
}

Var& Var::operator= (const bool& var) {
    this->type = BLN;
    this->data.bln = var;
    return *this;
}

Var& Var::operator= (const wstring& var) {
    this->type = STR;
    this->data.str = var;
    return *this;
}

Var& Var::operator= (const wchar_t* var) {
    this->type = STR;
    this->data.str = var;
    return *this;
}

Var operator+(const Var& a, const Var& b)
{
    if (a.type == NTG) {
        if (b.type == NTG) {
            Var result(a.data.ntg + b.data.ntg);
            return result;
        }
        else if (b.type == UNTG) {
            Var result(a.data.ntg + b.data.untg);
            return result;
        }
        else if (b.type == DBL) {
            Var result(a.data.ntg + b.data.dbl);
            return result;
        }
        else if (b.type == CHR) {
            Var result(a.data.ntg + b.data.chr);
            return result;
        }
        else if (b.type == UCHR) {
            Var result(a.data.ntg + b.data.uchr);
            return result;
        }
        else if (b.type == BLN) {
            Var result(a.data.ntg + b.data.bln);
            return result;
        }
        else if (b.type == STR) {
            Var result(to_wstring(a.data.ntg) + b.data.str);
            return result;
        }
        else if (b.type == NIL) {
            Var result;
            return result;
        }
        else {
            Var result;
            return result;
        }
    }
    else if (a.type == UNTG) {
        if (b.type == NTG) {
            Var result(a.data.untg + b.data.ntg);
            return result;
        }
        else if (b.type == UNTG) {
            Var result(a.data.untg + b.data.untg);
            return result;
        }
        else if (b.type == DBL) {
            Var result(a.data.untg + b.data.dbl);
            return result;
        }
        else if (b.type == CHR) {
            Var result(a.data.untg + b.data.chr);
            return result;
        }
        else if (b.type == UCHR) {
            Var result(a.data.untg + b.data.uchr);
            return result;
        }
        else if (b.type == BLN) {
            Var result(a.data.untg + b.data.bln);
            return result;
        }
        else if (b.type == STR) {
            Var result(to_wstring(a.data.untg) + b.data.str);
            return result;
        }
        else if (b.type == NIL) {
            Var result;
            return result;
        }
        else {
            Var result;
            return result;
        }
    }
    else if (a.type == DBL) {
        if (b.type == NTG) {
            Var result(a.data.dbl + b.data.ntg);
            return result;
        }
        else if (b.type == UNTG) {
            Var result(a.data.dbl + b.data.untg);
            return result;
        }
        else if (b.type == DBL) {
            Var result(a.data.dbl + b.data.dbl);
            return result;
        }
        else if (b.type == CHR) {
            Var result(a.data.dbl + b.data.chr);
            return result;
        }
        else if (b.type == UCHR) {
            Var result(a.data.dbl + b.data.uchr);
            return result;
        }
        else if (b.type == BLN) {
            Var result(a.data.dbl + b.data.bln);
            return result;
        }
        else if (b.type == STR) {
            Var result(to_wstring(a.data.dbl) + b.data.str);
            return result;
        }
        else if (b.type == NIL) {
            Var result;
            return result;
        }
        else {
            Var result;
            return result;
        }
    }
    else if (a.type == CHR) {
        if (b.type == NTG) {
            Var result(a.data.chr + b.data.ntg);
            return result;
        }
        else if (b.type == UNTG) {
            Var result(a.data.chr + b.data.untg);
            return result;
        }
        else if (b.type == DBL) {
            Var result(a.data.chr + b.data.dbl);
            return result;
        }
        else if (b.type == CHR) {
            Var result(a.data.chr + b.data.chr);
            return result;
        }
        else if (b.type == UCHR) {
            Var result(a.data.chr + b.data.uchr);
            return result;
        }
        else if (b.type == BLN) {
            Var result(a.data.chr + b.data.bln);
            return result;
        }
        else if (b.type == STR) {
            Var result(to_wstring(a.data.chr) + b.data.str);
            return result;
        }
        else if (b.type == NIL) {
            Var result;
            return result;
        }
        else {
            Var result;
            return result;
        }
    }
    else if (a.type == UCHR) {
        if (b.type == NTG) {
            Var result(a.data.uchr + b.data.ntg);
            return result;
        }
        else if (b.type == UNTG) {
            Var result(a.data.uchr + b.data.untg);
            return result;
        }
        else if (b.type == DBL) {
            Var result(a.data.uchr + b.data.dbl);
            return result;
        }
        else if (b.type == CHR) {
            Var result(a.data.uchr + b.data.chr);
            return result;
        }
        else if (b.type == UCHR) {
            Var result(a.data.uchr + b.data.uchr);
            return result;
        }
        else if (b.type == BLN) {
            Var result(a.data.uchr + b.data.bln);
            return result;
        }
        else if (b.type == STR) {
            Var result(to_wstring(a.data.uchr) + b.data.str);
            return result;
        }
        else if (b.type == NIL) {
            Var result;
            return result;
        }
        else {
            Var result;
            return result;
        }
    }
    else if (a.type == BLN) {
        if (b.type == NTG) {
            Var result(a.data.bln + b.data.ntg);
            return result;
        }
        else if (b.type == UNTG) {
            Var result(a.data.bln + b.data.untg);
            return result;
        }
        else if (b.type == DBL) {
            Var result(a.data.bln + b.data.dbl);
            return result;
        }
        else if (b.type == CHR) {
            Var result(a.data.bln + b.data.chr);
            return result;
        }
        else if (b.type == UCHR) {
            Var result(a.data.bln + b.data.uchr);
            return result;
        }
        else if (b.type == BLN) {
            Var result(a.data.bln + b.data.bln);
            return result;
        }
        else if (b.type == STR) {
            if (a.data.bln) {
                Var result(L"true" + b.data.str);
                return result;
            }
            else {
                Var result(L"false" + b.data.str);
                return result;
            }
        }
        else if (b.type == NIL) {
            Var result;
            return result;
        }
        else {
            Var result;
            return result;
        }
    }
    else {
        Var result;
        return result;
    }

}
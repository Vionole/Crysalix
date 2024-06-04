
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include "VarbleClass.h"

using namespace std;

Varble::Varble() {
    this->type = NIL;
}

Varble::Varble(unsigned long long int untg) {
    this->type = UNTG;
    this->data.untg = untg;
}

Varble::Varble(long long int ntg) {
    this->type = NTG;
    this->data.ntg = ntg;
}

Varble::Varble(long int ntg) {
    this->type = NTG;
    this->data.ntg = (long long int)ntg;
}

Varble::Varble(int ntg) {
    this->type = NTG;
    this->data.ntg = (long long int)ntg;
}

Varble::Varble(short int ntg) {
    this->type = NTG;
    this->data.ntg = (long long int)ntg;
}

Varble::Varble(unsigned short int ntg) {
    this->type = NTG;
    this->data.ntg = (long long int)ntg;
}

Varble::Varble(unsigned int ntg) {
    this->type = NTG;
    this->data.ntg = (long long int)ntg;
}

Varble::Varble(unsigned long int ntg) {
    this->type = NTG;
    this->data.ntg = (long long int)ntg;
}

Varble::Varble(long double dbl) {
    this->type = DBL;
    this->data.dbl = dbl;
}

Varble::Varble(double dbl) {
    this->type = DBL;
    this->data.dbl = (long double)dbl;
}

Varble::Varble(float dbl) {
    this->type = DBL;
    this->data.dbl = (long double)dbl;
}

Varble::Varble(char chr) {
    this->type = CHR;
    this->data.chr = chr;
}

Varble::Varble(unsigned char uchr) {
    this->type = UCHR;
    this->data.uchr = uchr;
}

Varble::Varble(bool bln) {
    this->type = BLN;
    this->data.bln = bln;
}

Varble::Varble(wstring str) {
    this->type = STR;
    this->data.str = str;
}

Varble::Varble(const wchar_t* str) {
    this->type = STR;
    this->data.str = str;
}

Varble::Varble(wstring t, int i) {
    if (t == L"array") {
        this->type = ARR;
        this->arr = vector<Varble>(i);
    } 
    else if (t == L"map" && i == 0) {
        this->type = MAP;
        this->mp = map<wstring, Varble>();
    }
    else {
        this->type == UNKNOWN;
    }
}

long long int Varble::getInt() {
    if (this->type == NTG) {
        return this->data.ntg;
    }
    else {
        throw wstring{ L"Переменная не является знаковой целочисленной" };
    }
}

unsigned long long int Varble::getUInt() {
    if (this->type == UNTG) {
        return this->data.untg;
    }
    else {
        throw wstring{ L"Переменная не является беззнаковой целочисленной" };
    }
}

long double Varble::getDouble() {
    if (this->type == DBL) {
        return this->data.dbl;
    }
    else {
        throw wstring{ L"Переменная не является числом с плавающей точкой" };
    }
}

char Varble::getChar() {
    if (this->type == CHR) {
        return this->data.chr;
    }
    else {
        throw wstring{ L"Переменная не является символьной" };
    }
}

unsigned char Varble::getUChar() {
    if (this->type == UCHR) {
        return this->data.uchr;
    }
    else {
        throw wstring{ L"Переменная не является беззнаковой символьной" };
    }
}

bool Varble::getBool() {
    if (this->type == BLN) {
        return this->data.bln;
    }
    else {
        throw wstring{ L"Переменная не является булевой" };
    }
}

wstring Varble::getWStr() {
    if (this->type == STR) {
        return this->data.str;
    }
    else {
        throw wstring{ L"Переменная не является строкой" };
    }
}

vector<Varble> Varble::getArr() {
    if (this->type == ARR) {
        return this->arr;
    }
    else {
        throw wstring{ L"Переменная не является массивом" };
    }
}

map<wstring, Varble> Varble::getMap() {
    if (this->type == MAP) {
        return this->mp;
    }
    else {
        throw wstring{ L"Переменная не является словарем" };
    }
}

void Varble::print() {
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

wstring Varble::typeOf() {
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

Varble& Varble::operator[](int ind) {
    return this->arr[ind];
}
Varble& Varble::operator[](Varble v) {
    return this->arr[v.getInt()];
}



wostream& operator<< (wostream& wos, const Varble& var)
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

Varble& Varble::operator= (const Varble& var) {
    this->type = var.type;
    this->data.untg = var.data.untg;
    this->data.ntg = var.data.ntg;
    this->data.dbl = var.data.dbl;
    this->data.chr = var.data.chr;
    this->data.uchr = var.data.uchr;
    this->data.bln = var.data.bln;
    this->data.str = var.data.str;
    return *this; 
}

Varble& Varble::operator= (const unsigned long long int& var) {
    this->type = UNTG;
    this->data.untg = var;
    return *this;
}

Varble& Varble::operator= (const unsigned long int& var) {
    this->type = NTG;
    this->data.ntg = var;
    return *this;
}

Varble& Varble::operator= (const unsigned int& var) {
    this->type = NTG;
    this->data.ntg = var;
    return *this;
}

Varble& Varble::operator= (const unsigned short int& var) {
    this->type = NTG;
    this->data.ntg = var;
    return *this;
}

Varble& Varble::operator= (const long long int& var) {
    this->type = NTG;
    this->data.ntg = var;
    return *this;
}

Varble& Varble::operator= (const long int& var) {
    this->type = NTG;
    this->data.ntg = var;
    return *this;
}

Varble& Varble::operator= (const int& var) {
    this->type = NTG;
    this->data.ntg = var;
    return *this;
}

Varble& Varble::operator= (const short int& var) {
    this->type = NTG;
    this->data.ntg = var;
    return *this;
}

Varble& Varble::operator= (const long double& var) {
    this->type = DBL;
    this->data.dbl = var;
    return *this;
}

Varble& Varble::operator= (const double& var) {
    this->type = DBL;
    this->data.dbl = (long double)var;
    return *this;
}

Varble& Varble::operator= (const float& var) {
    this->type = DBL;
    this->data.dbl = (const long double)var;
    return *this;
}

Varble& Varble::operator= (const char& var) {
    this->type = CHR;
    this->data.chr = var;
    return *this;
}

Varble& Varble::operator= (const unsigned char& var) {
    this->type = UCHR;
    this->data.uchr = var;
    return *this;
}

Varble& Varble::operator= (const bool& var) {
    this->type = BLN;
    this->data.bln = var;
    return *this;
}

Varble& Varble::operator= (const wstring& var) {
    this->type = STR;
    this->data.str = var;
    return *this;
}

Varble& Varble::operator= (const wchar_t* var) {
    this->type = STR;
    this->data.str = var;
    return *this;
}

Varble operator+(const Varble& a, const Varble& b)
{
    if (a.type == NTG) {
        if (b.type == NTG) {
            Varble result(a.data.ntg + b.data.ntg);
            return result;
        }
        else if (b.type == UNTG) {
            Varble result(a.data.ntg + b.data.untg);
            return result;
        }
        else if (b.type == DBL) {
            Varble result(a.data.ntg + b.data.dbl);
            return result;
        }
        else if (b.type == CHR) {
            Varble result(a.data.ntg + b.data.chr);
            return result;
        }
        else if (b.type == UCHR) {
            Varble result(a.data.ntg + b.data.uchr);
            return result;
        }
        else if (b.type == BLN) {
            Varble result(a.data.ntg + b.data.bln);
            return result;
        }
        else if (b.type == STR) {
            Varble result(to_wstring(a.data.ntg) + b.data.str);
            return result;
        }
        else if (b.type == NIL) {
            Varble result;
            return result;
        }
        else {
            Varble result;
            return result;
        }
    }
    else if (a.type == UNTG) {
        if (b.type == NTG) {
            Varble result(a.data.untg + b.data.ntg);
            return result;
        }
        else if (b.type == UNTG) {
            Varble result(a.data.untg + b.data.untg);
            return result;
        }
        else if (b.type == DBL) {
            Varble result(a.data.untg + b.data.dbl);
            return result;
        }
        else if (b.type == CHR) {
            Varble result(a.data.untg + b.data.chr);
            return result;
        }
        else if (b.type == UCHR) {
            Varble result(a.data.untg + b.data.uchr);
            return result;
        }
        else if (b.type == BLN) {
            Varble result(a.data.untg + b.data.bln);
            return result;
        }
        else if (b.type == STR) {
            Varble result(to_wstring(a.data.untg) + b.data.str);
            return result;
        }
        else if (b.type == NIL) {
            Varble result;
            return result;
        }
        else {
            Varble result;
            return result;
        }
    }
    else if (a.type == DBL) {
        if (b.type == NTG) {
            Varble result(a.data.dbl + b.data.ntg);
            return result;
        }
        else if (b.type == UNTG) {
            Varble result(a.data.dbl + b.data.untg);
            return result;
        }
        else if (b.type == DBL) {
            Varble result(a.data.dbl + b.data.dbl);
            return result;
        }
        else if (b.type == CHR) {
            Varble result(a.data.dbl + b.data.chr);
            return result;
        }
        else if (b.type == UCHR) {
            Varble result(a.data.dbl + b.data.uchr);
            return result;
        }
        else if (b.type == BLN) {
            Varble result(a.data.dbl + b.data.bln);
            return result;
        }
        else if (b.type == STR) {
            Varble result(to_wstring(a.data.dbl) + b.data.str);
            return result;
        }
        else if (b.type == NIL) {
            Varble result;
            return result;
        }
        else {
            Varble result;
            return result;
        }
    }
    else if (a.type == CHR) {
        if (b.type == NTG) {
            Varble result(a.data.chr + b.data.ntg);
            return result;
        }
        else if (b.type == UNTG) {
            Varble result(a.data.chr + b.data.untg);
            return result;
        }
        else if (b.type == DBL) {
            Varble result(a.data.chr + b.data.dbl);
            return result;
        }
        else if (b.type == CHR) {
            Varble result(a.data.chr + b.data.chr);
            return result;
        }
        else if (b.type == UCHR) {
            Varble result(a.data.chr + b.data.uchr);
            return result;
        }
        else if (b.type == BLN) {
            Varble result(a.data.chr + b.data.bln);
            return result;
        }
        else if (b.type == STR) {
            Varble result(to_wstring(a.data.chr) + b.data.str);
            return result;
        }
        else if (b.type == NIL) {
            Varble result;
            return result;
        }
        else {
            Varble result;
            return result;
        }
    }
    else if (a.type == UCHR) {
        if (b.type == NTG) {
            Varble result(a.data.uchr + b.data.ntg);
            return result;
        }
        else if (b.type == UNTG) {
            Varble result(a.data.uchr + b.data.untg);
            return result;
        }
        else if (b.type == DBL) {
            Varble result(a.data.uchr + b.data.dbl);
            return result;
        }
        else if (b.type == CHR) {
            Varble result(a.data.uchr + b.data.chr);
            return result;
        }
        else if (b.type == UCHR) {
            Varble result(a.data.uchr + b.data.uchr);
            return result;
        }
        else if (b.type == BLN) {
            Varble result(a.data.uchr + b.data.bln);
            return result;
        }
        else if (b.type == STR) {
            Varble result(to_wstring(a.data.uchr) + b.data.str);
            return result;
        }
        else if (b.type == NIL) {
            Varble result;
            return result;
        }
        else {
            Varble result;
            return result;
        }
    }
    else if (a.type == BLN) {
        if (b.type == NTG) {
            Varble result(a.data.bln + b.data.ntg);
            return result;
        }
        else if (b.type == UNTG) {
            Varble result(a.data.bln + b.data.untg);
            return result;
        }
        else if (b.type == DBL) {
            Varble result(a.data.bln + b.data.dbl);
            return result;
        }
        else if (b.type == CHR) {
            Varble result(a.data.bln + b.data.chr);
            return result;
        }
        else if (b.type == UCHR) {
            Varble result(a.data.bln + b.data.uchr);
            return result;
        }
        else if (b.type == BLN) {
            Varble result(a.data.bln + b.data.bln);
            return result;
        }
        else if (b.type == STR) {
            if (a.data.bln) {
                Varble result(L"true" + b.data.str);
                return result;
            }
            else {
                Varble result(L"false" + b.data.str);
                return result;
            }
        }
        else if (b.type == NIL) {
            Varble result;
            return result;
        }
        else {
            Varble result;
            return result;
        }
    }
    else {
        Varble result;
        return result;
    }

}
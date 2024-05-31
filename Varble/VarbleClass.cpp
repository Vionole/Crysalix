
#include <vector>
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
        wcout << "NULL";
        break;
    default:
        wcout << "UNKNOWN";
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
    default:
        result = L"UNKNOWN";
        return result;
    }
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
    default:
        return wos << L"UNKNOWN";
    }
}

Varble& Varble::operator= (Varble& var) {
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

Varble& Varble::operator= (unsigned long long int& var) {
    this->type = UNTG;
    this->data.untg = var;
    return *this;
}

Varble& Varble::operator= (unsigned long int& var) {
    this->type = NTG;
    this->data.ntg = var;
    return *this;
}

Varble& Varble::operator= (unsigned int& var) {
    this->type = NTG;
    this->data.ntg = var;
    return *this;
}

Varble& Varble::operator= (unsigned short int& var) {
    this->type = NTG;
    this->data.ntg = var;
    return *this;
}

Varble& Varble::operator= (long long int& var) {
    this->type = NTG;
    this->data.ntg = var;
    return *this;
}

Varble& Varble::operator= (long int& var) {
    this->type = NTG;
    this->data.ntg = var;
    return *this;
}

Varble& Varble::operator= (int& var) {
    this->type = NTG;
    this->data.ntg = var;
    return *this;
}

Varble& Varble::operator= (short int& var) {
    this->type = NTG;
    this->data.ntg = var;
    return *this;
}

Varble& Varble::operator= (long double& var) {
    this->type = DBL;
    this->data.dbl = var;
    return *this;
}

Varble& Varble::operator= (double& var) {
    this->type = DBL;
    this->data.dbl = (long double)var;
    return *this;
}

Varble& Varble::operator= (float& var) {
    this->type = DBL;
    this->data.dbl = (long double)var;
    return *this;
}

Varble& Varble::operator= (char& var) {
    this->type = CHR;
    this->data.chr = var;
    return *this;
}

Varble& Varble::operator= (unsigned char& var) {
    this->type = UCHR;
    this->data.uchr = var;
    return *this;
}

Varble& Varble::operator= (bool& var) {
    this->type = BLN;
    this->data.bln = var;
    return *this;
}

Varble& Varble::operator= (wstring& var) {
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
    else {
        Varble result;
        return result;
    }

}
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include "VarClass.h"
using namespace std;


int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);

    try {
        //unsigned long long int var = 18446744073709551600ULL;
        wstring var = L"abcd";
        Var v(L"array", 5);
        Var v2 = 2;
        v[v2] = L"123abc";
        wcout << L"Значение переменной: " << v[v2] << L" тип: " << v.typeOf() << endl;
    }
    catch (const std::wstring& error_message) {
        wcout << error_message << endl;
    }

    return 0;
}

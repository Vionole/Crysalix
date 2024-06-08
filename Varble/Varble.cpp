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
        wstring var = L"   abcd    ";
        Var v = 2;
        wcout << L"Значение переменной: " << v.slice(1, Var(2))[0] << L" тип: " << v.typeOf() << endl;
    }
    catch (const std::wstring& error_message) {
        wcout << error_message << endl;
    }

    return 0;
}

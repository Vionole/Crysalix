#include <iostream>
#include <io.h>
#include <fcntl.h>
#include "VarbleClass.h"
using namespace std;


int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stderr), _O_U16TEXT);

    try {
        //unsigned long long int var = 18446744073709551600ULL;
        unsigned char var = 'X';
        Varble v(L"map", 0);

        wcout << L"Значение переменной: " << v.toUNTG() << L" тип: " << v.typeOf() << endl;
    }
    catch (const std::wstring& error_message) {
        wcout << error_message << endl;
    }

    return 0;
}

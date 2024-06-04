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
        bool var = true;
        Varble v = 6;
        Varble v2 = var;
        Varble v3 = var;
        Varble v4 = v + v2;
        Varble v5(L"array", 5);
        v5[v] = 121;
        wcout << L"Значение переменной: " << v5[v] << L" тип: " << v3.typeOf() << endl;
    }
    catch (const std::wstring& error_message) {
        wcout << error_message << endl;
    }

    return 0;
}

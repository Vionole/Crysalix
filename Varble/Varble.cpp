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
    unsigned long long int var = 123;
    Varble v = true;
    Varble v2 = var;
    Varble v3 = 2;
    Varble v4 = v + v2;
    Varble v5(L"array", 5);
    v5[0] = 121;
    wcout << L"Значение переменной: " << v5[0] << L" тип: " << v5.typeOf() << endl;
    return 0;
}

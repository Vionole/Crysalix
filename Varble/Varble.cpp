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
    wstring var = L"str";
    Varble v = 10ULL;
    Varble v2;
    Varble v3 = 2;
    Varble v4 = v + v2;

    wcout << L"Значение переменной: " << v4 << L" тип: " << v4.typeOf() << endl;
    return 0;
}

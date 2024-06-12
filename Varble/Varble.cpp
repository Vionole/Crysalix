#include <iostream>
#include <io.h>
#include <fcntl.h>
#include "VarClass.h"
using namespace std;


int main()
{
    Unicode();

    try {
        //unsigned long long int var = 18446744073709551600ULL;
        Var v1((bool)false);
        Var v2(L"123");
        wcout << L"Значение переменной: " << v1 + v2 + v2 << L" тип: " << (v1 + v2).typeOf() << endl;
    }
    catch (const std::wstring& error_message) {
        wcout << endl << error_message;
    }

    return 0;
}

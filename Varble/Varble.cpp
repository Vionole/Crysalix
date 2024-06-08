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
        wstring var = L"Hello, World!";
        map<wstring, Var> m;
        m[L"first"] = L"123";
        vector<Var> vec = {1, 2, 3};
        Var v = var;
      
        wcout << L"Значение переменной: " << v.split(Var(L","))[1] << L" тип: " << v.typeOf() << endl;
    }
    catch (const std::wstring& error_message) {
        wcout << error_message << endl;
    }

    return 0;
}

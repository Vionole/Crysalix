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
        m[L"second"] = L"321";
        vector<Var> vec = {1, 2, 3};
        Var v;
        v = 2;

        v.print();
        v = v.join(wstring(L" | "));
        v.print();
        wcout << L"Значение переменной: " << v << L" тип: " << v.typeOf() << endl;
    }
    catch (const std::wstring& error_message) {
        wcout << endl << error_message;
    }

    return 0;
}

#include <iostream>
#include <io.h>
#include <sstream>
#include <fstream>
#include <codecvt>
#include <fcntl.h>
#include "VarClass.h"

using namespace std;

int main()
{
    Unicode();

    //Чтение файла в кодировке UTF-8
    std::wifstream infile("file.txt");
    if (infile) {
        infile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
        std::wstringstream wss;
        wss << infile.rdbuf();
        std::wcout << wss.str();
    }
    infile.close();

    try {
        //unsigned long long int var = 18446744073709551600ULL;
        Var v1(L"map", 0);
        Var v2(L"map", 0);
        v1.insert(L"first", Var(2));
        v1.insert(L"second", Var(L"abc"));
        v2.insert(L"apple", Var(true));
        v2.insert(L"orange", Var(3.14));
        v1.print();
        v2.print();
        v1.merge(v2);
        v1.print();
        wcout << L"Значение переменной: " << !v1 << L" тип: " << v2.typeOf() << endl;
    }
    catch (const std::wstring& error_message) {
        wcout << endl << error_message;
    }
    system("pause");
    return 0;
}


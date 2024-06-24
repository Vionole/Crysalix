#include <iostream>
#include <io.h>
#include <sstream>
#include <fstream>
#include <codecvt>
#include <fcntl.h>
#include "VarClass.h"
#include "Machine.h"

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
        map<wstring, Var> map;
        Machine mchn(map, true);
                        
        vector<Var> vecnop;
        mchn.instructions.push_back(new InstructNOP(vecnop));
        mchn.instructions.push_back(new InstructNOP(vecnop));

        vector<Var> veclbl;
        veclbl.push_back(Var(L"&label1"));
        mchn.instructions.push_back(new InstructLBL(veclbl));

        vector<Var> vecvar;
        vecvar.push_back(Var(L"$name"));
        vecvar.push_back(Var(123));
        mchn.instructions.push_back(new InstructVAR(vecvar));

        vector<Var> vecvar2;
        vecvar2.push_back(Var(L"$second"));
        vecvar2.push_back(Var(L"$name"));
        mchn.instructions.push_back(new InstructVAR(vecvar2));

        vector<Var> vecprint;
        vecprint.push_back(Var(L"\nПервое значение: "));
        vecprint.push_back(Var(L"$second"));
        vecprint.push_back(Var(L" второе значение: "));
        vecprint.push_back(Var(L"$name"));
        vecprint.push_back(Var(L" "));
        vecprint.push_back(Var(true));
        vecprint.push_back(Var(L"\n"));
        mchn.instructions.push_back(new InstructPRINT(vecprint));


        vector<Var> vecfree;
        vecfree.push_back(Var(L"$name"));
        vecfree.push_back(Var(L"$second"));
        mchn.instructions.push_back(new InstructFREE(vecfree));


        vector<Var> veclbl2;
        veclbl2.push_back(Var(L"&label2"));
        mchn.instructions.push_back(new InstructLBL(veclbl2));

        /*vector<Var> vecjmp;
        vecjmp.push_back(Var(L"&label1"));
        mchn.instructions.push_back(new InstructJMP(vecjmp));*/

        vector<Var> vecend;
        vecend.push_back(Var(L"name"));
        mchn.instructions.push_back(new InstructEND(vecend));

        mchn.prepare();
        wcout << L"Программа выполнилась и вернула значение: " << mchn.go() << endl;
    }
    catch (const std::wstring& error_message) {
        wcout << endl << error_message;
    }
    system("pause");
    return 0;
}


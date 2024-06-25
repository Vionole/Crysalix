#include <iostream>
#include <io.h>
#include <sstream>
#include <fstream>
#include <codecvt>
#include <fcntl.h>
#include "VarClass.h"
#include "Machine.h"
#include "Parser.h"

using namespace std;

int main()
{
    Unicode();

    try {
        clock_t start = clock();
        map<wstring, Var> map;
        Machine mchn(map, false);

        Parser p(L"file.vrb");
        p.fileLoad();
        p.parse(mchn);

        mchn.prepare();
        wcout << endl << mchn.go() << endl;

        clock_t end = clock();
        double seconds = (double)(end - start) / CLOCKS_PER_SEC;
        wcout << seconds << L" sec." << endl;
    }
    catch (const std::wstring& error_message) {
        wcout << endl << error_message << endl;
    }
    system("pause");
    return 0;
}


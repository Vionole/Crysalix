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
        map<wstring, Var> map;
        Machine mchn(map, false);

        Parser p(L"file.vrb");
        p.fileLoad();
        p.parse(mchn);

        mchn.prepare();
        wcout << endl << mchn.go() << endl;
    }
    catch (const std::wstring& error_message) {
        wcout << endl << error_message << endl;
    }
    system("pause");
    return 0;
}


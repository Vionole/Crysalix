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
        
    }
    catch (const std::wstring& error_message) {
        wcout << endl << error_message;
    }
    system("pause");
    return 0;
}


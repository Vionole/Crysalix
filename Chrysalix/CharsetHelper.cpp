#pragma once
#include <sstream>
#include <fstream>
#include <codecvt>
#include <io.h>
#include <fcntl.h>


#include "Helpers.h"

wstring loadFile(wstring filename) {
    wstring file_content = L"";
    std::wifstream infile(filename);

    if (infile) {
        infile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
        std::wstringstream wss;
        wss << infile.rdbuf();
        file_content = wss.str();
    }
    else {
        throw wstring{ L"Файл " + filename + L" не найден\n" };
    }
    infile.close();

    return file_content;
}

void Unicode() {
    int result = _setmode(_fileno(stdout), _O_U16TEXT);
    result = _setmode(_fileno(stdin), _O_U16TEXT);
    result = _setmode(_fileno(stderr), _O_U16TEXT);
}
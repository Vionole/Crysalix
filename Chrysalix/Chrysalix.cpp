#pragma once

#include <iostream>
#include <io.h>
#include <sstream>
#include <fstream>
#include <codecvt>
#include <fcntl.h>
#include "windows.h"
#include <chrono>

#include "Machine.h"
#include "Parser.h"

using namespace std;

int main(int argc, char* argv[])
{
    
    wstring filename = L"";
    if (argc < 2) {
        //Если не передан параметр при запуске, смотрим файл настроек
        std::wifstream infile(L"autoexec.ini");
        if (infile) {
            infile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
            std::wstringstream wss;
            wss << infile.rdbuf();
            filename = wss.str();
        }
        //Если файла настроек нет, выводим версию программы
        else {
            wcout << L"0.1 alpha";
            return 0;
        }
        infile.close();

    }

    //берем параметр при вызове, это имя файла скрипта для запуска
    if (filename == L"") {
        filename = wstring(argv[1], argv[1] + strlen(argv[1]));
    }

    //Поднятие приоритета процесса
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

    //Кодировка Unicode для читаемых, записывемых и отображаемых символов
    Unicode();

    try {

        map<wstring, Var> map;
        Machine mchn(map, false);

        //Загружаем и парсим исходный код
        Parser p(filename);
        p.fileLoad();
        //auto begin = chrono::high_resolution_clock::now();
        p.parse(mchn);
        //auto end = chrono::high_resolution_clock::now();
        //auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        //std::wcout << L"Время обработки исходного кода: " << to_wstring((double)elapsed_ms.count() / 1000000.0) << L"sec\n";

        //Выполняем код
        //begin = chrono::high_resolution_clock::now();
        mchn.prepare();
        mchn.go();
        //end = chrono::high_resolution_clock::now();
        //elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        //std::wcout << L"Время выполнения: " << to_wstring((double)elapsed_ms.count() / 1000000.0) << L"sec\n";
    }
    catch (const std::wstring& error_message) {
        wcout << endl << error_message << endl;
    }
    
    return 0;
}


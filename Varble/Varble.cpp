#include <iostream>
#include <io.h>
#include <sstream>
#include <fstream>
#include <codecvt>
#include <fcntl.h>
#include "windows.h"
#include <chrono>
#include "VarClass.h"
#include "Machine.h"
#include "Parser.h"

using namespace std;

int main()
{
    //Поднятие приоритета процесса
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

    //Кодировка Unicode для читаемых, записывемых и отображаемых символов
    Unicode();

    try {

        map<wstring, Var> map;
        Machine mchn(map, false);

        //Загружаем и парсим исходный код
        Parser p(L"file.vrb");
        p.fileLoad();
        auto begin = chrono::high_resolution_clock::now();
        p.parse(mchn);
        auto end = chrono::high_resolution_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::wcout << L"Время обработки исходного кода: " << to_wstring((double)elapsed_ms.count() / 1000000.0) << L"sec\n";

        //Выполняем код
        begin = chrono::high_resolution_clock::now();
        mchn.prepare();
        mchn.go();
        end = chrono::high_resolution_clock::now();
        elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::wcout << L"Время выполнения: " << to_wstring((double)elapsed_ms.count() / 1000000.0) << L"sec\n";
    }
    catch (const std::wstring& error_message) {
        wcout << endl << error_message << endl;
    }
    system("pause");
    return 0;
}


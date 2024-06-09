#pragma once
#include <string>
#include <vector>
#include <map>
using namespace std;

struct Data {
    long long int ntg = 0;
    unsigned long long int untg = 0;
    long double dbl = 0;
    char chr = 0;
    unsigned char uchr = 0;
    bool bln = false;
    wstring str = L"";
};

enum Type {
    NTG,
    UNTG,
    DBL,
    CHR,
    UCHR,
    BLN,
    STR,
    NIL,
    UNKNOWN,
    ARR,
    MAP
};

class Var {
private:
    Data data;
    Type type;
    vector<Var> arr;
    map<wstring, Var> mp;
public:
    //Конструкторы для обычны типов данных
    Var();
    Var(unsigned long long int untg);
    Var(long long int ntg);
    Var(long int ntg);
    Var(int ntg);
    Var(short int ntg);
    Var(unsigned long int ntg);
    Var(unsigned int ntg);
    Var(unsigned short int ntg);

    Var(long double dbl);
    Var(double dbl);
    Var(float dbl);

    Var(char chr);
    Var(unsigned char uchr);

    Var(bool bln);

    Var(wstring str);
    Var(const wchar_t* str); 
    
    Var(wstring t, int i);

    Var(vector<Var> v);
    Var(map<wstring, Var> m);

    //Геттеры обычных типов данных
    long long int getInt();
    unsigned long long int getUInt();
    long double getDouble();
    char getChar();
    unsigned char getUChar();
    bool getBool();
    wstring getWStr();
    vector<Var> getArr();
    map<wstring, Var> getMap();

    //Приведение типов
    Var toNTG();
    Var toUNTG();
    Var toDBL();
    Var toCHR();
    Var toUCHR();
    Var toBLN();
    Var toSTR();
    Var toARR();

    //Вывод данных и типа
    void print();
    wstring typeOf();

    //Работа с массивами, словарями и строками
    Var& operator[](int ind);
    Var& operator[](const wchar_t* str);
    Var& operator[](wstring str);
    Var& operator[](Var v);

    Var len();
    Var rev();
    Var in(Var sent);
    Var in(const wchar_t* sent);
    Var in(wstring sent);

    Var slice(int x, int y);
    Var slice(Var x, Var y);

    Var ltrim();
    Var rtrim();
    Var trim();
    Var repl(Var substr, Var newsubstr);
    Var repl(const wchar_t* substr, const wchar_t* newsubstr);
    Var repl(wstring substr, const wstring newsubstr);
    Var upper();
    Var lower();

    void pushb(Var v);
    void popb();
    void clear();
    void erase(int x);
    void erase(Var x);

    Var split(Var delim);

    //Вывод данных через поток
    friend wostream& operator<< (wostream& os, const Var& var);

    //Перегрузка присваивания
    Var& operator= (const Var& var);

    Var& operator= (const unsigned long long int& var);
    Var& operator= (const unsigned long int& var);
    Var& operator= (const unsigned int& var);
    Var& operator= (const unsigned short int& var);
    Var& operator= (const long long int& var);
    Var& operator= (const long int& var);
    Var& operator= (const int& var);
    Var& operator= (const short int& var);

    Var& operator= (const long double& var);
    Var& operator= (const double& var);
    Var& operator= (const float& var);

    Var& operator= (const char& var);
    Var& operator= (const unsigned char& var);

    Var& operator= (const bool& var);

    Var& operator= (const wstring& var);
    Var& operator= (const wchar_t* var);

    //Математические операторы
    friend Var operator+(const Var& a, const Var& b);
};


void Unicode();
void swap(Var& a, Var& b);

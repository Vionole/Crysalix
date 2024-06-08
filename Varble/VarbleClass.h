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

class Varble {
private:
    Data data;
    Type type;
    vector<Varble> arr;
    map<wstring, Varble> mp;
public:
    Varble();
    Varble(unsigned long long int untg);
    Varble(long long int ntg);
    Varble(long int ntg);
    Varble(int ntg);
    Varble(short int ntg);
    Varble(unsigned long int ntg);
    Varble(unsigned int ntg);
    Varble(unsigned short int ntg);

    Varble(long double dbl);
    Varble(double dbl);
    Varble(float dbl);

    Varble(char chr);
    Varble(unsigned char uchr);

    Varble(bool bln);

    Varble(wstring str);
    Varble(const wchar_t* str); 
    
    Varble(wstring t, int i);

    long long int getInt();
    unsigned long long int getUInt();
    long double getDouble();
    char getChar();
    unsigned char getUChar();
    bool getBool();
    wstring getWStr();
    vector<Varble> getArr();
    map<wstring, Varble> getMap();

    Varble toNTG();
    Varble toUNTG();
    Varble toDBL();

    void print();
    wstring typeOf();

    Varble& operator[](int ind);
    Varble& operator[](Varble v);

    friend wostream& operator<< (wostream& os, const Varble& var);

    Varble& operator= (const Varble& var);

    Varble& operator= (const unsigned long long int& var);
    Varble& operator= (const unsigned long int& var);
    Varble& operator= (const unsigned int& var);
    Varble& operator= (const unsigned short int& var);
    Varble& operator= (const long long int& var);
    Varble& operator= (const long int& var);
    Varble& operator= (const int& var);
    Varble& operator= (const short int& var);

    Varble& operator= (const long double& var);
    Varble& operator= (const double& var);
    Varble& operator= (const float& var);

    Varble& operator= (const char& var);
    Varble& operator= (const unsigned char& var);

    Varble& operator= (const bool& var);

    Varble& operator= (const wstring& var);
    Varble& operator= (const wchar_t* var);


    friend Varble operator+(const Varble& a, const Varble& b);
};
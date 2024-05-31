#pragma once
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
    NIL
};

class Varble {
private:
    Data data;
    Type type;
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

    void print();
    wstring typeOf();
    friend wostream& operator<< (wostream& os, const Varble& var);

    Varble& operator= (Varble& var);

    Varble& operator= (unsigned long long int& var);
    Varble& operator= (unsigned long int& var);
    Varble& operator= (unsigned int& var);
    Varble& operator= (unsigned short int& var);
    Varble& operator= (long long int& var);
    Varble& operator= (long int& var);
    Varble& operator= (int& var);
    Varble& operator= (short int& var);

    Varble& operator= (long double& var);
    Varble& operator= (double& var);
    Varble& operator= (float& var);

    Varble& operator= (char& var);
    Varble& operator= (unsigned char& var);

    Varble& operator= (bool& var);

    Varble& operator= (wstring& var);
    Varble& operator= (const wchar_t* var);

    friend Varble operator+(const Varble& a, const Varble& b);
};
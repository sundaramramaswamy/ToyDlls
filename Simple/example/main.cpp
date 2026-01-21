#include <windows.h>
#include <iostream>
#include "SimpleMath.h"

typedef int (*AddFunc)(int, int);
typedef int (*SubFunc)(int, int);

int main()
{
    HMODULE hDll = LoadLibrary(L"SimpleMath.dll");
    if (!hDll)
    {
        std::cerr << "Failed to load SimpleMath.dll" << std::endl;
        return 1;
    }

    AddFunc addFunc = (AddFunc)GetProcAddress(hDll, "add");
    SubFunc subFunc = (SubFunc)GetProcAddress(hDll, "sub");

    if (!addFunc || !subFunc)
    {
        std::cerr << "Failed to get function addresses" << std::endl;
        FreeLibrary(hDll);
        return 1;
    }

    int a = 10;
    int b = 5;

    std::cout << a << " + " << b << " = " << addFunc(a, b) << std::endl;
    std::cout << a << " - " << b << " = " << subFunc(a, b) << std::endl;

    FreeLibrary(hDll);
    return 0;
}

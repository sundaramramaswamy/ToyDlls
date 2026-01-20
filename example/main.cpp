#include <windows.h>
#include <iostream>

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

    AddFunc add = (AddFunc)GetProcAddress(hDll, "add");
    SubFunc sub = (SubFunc)GetProcAddress(hDll, "sub");

    if (!add || !sub)
    {
        std::cerr << "Failed to get function addresses" << std::endl;
        FreeLibrary(hDll);
        return 1;
    }

    int a = 10;
    int b = 5;

    std::cout << a << " + " << b << " = " << add(a, b) << std::endl;
    std::cout << a << " - " << b << " = " << sub(a, b) << std::endl;

    FreeLibrary(hDll);
    return 0;
}

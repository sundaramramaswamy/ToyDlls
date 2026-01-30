#include <iostream>
#include "StaticMath.h"

int main()
{
    int a = 10;
    int b = 5;

    std::cout << a << " + " << b
              << " = "
              << add(a, b)
              << std::endl;
    std::cout << a << " - " << b
              << " = "
              << sub(a, b)
              << std::endl;

    return 0;
}

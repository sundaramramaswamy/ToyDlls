#include <windows.h>
#include <iostream>
#include "ComMath.h"

int main()
{
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(hr))
    {
        std::cerr << "Failed to initialize COM: 0x" << std::hex << hr << std::endl;
        return 1;
    }

    std::cout << "=== ComMath C++ Client ===" << std::endl;
    
    IMath* pMath = nullptr;
    hr = CoCreateInstance(CLSID_Math, nullptr, CLSCTX_INPROC_SERVER, IID_IMath, (void**)&pMath);
    
    if (FAILED(hr))
    {
        std::cerr << "Failed to create COM object: 0x" << std::hex << hr << std::endl;
        CoUninitialize();
        return 1;
    }
    
    std::cout << "\nTesting Math operations:" << std::endl;
    
    // Test Add
    double result = 0;
    hr = pMath->Add(10.5, 20.3, &result);
    if (SUCCEEDED(hr))
        std::cout << "  10.5 + 20.3 = " << result << std::endl;
    
    // Test Subtract
    hr = pMath->Subtract(50.0, 12.5, &result);
    if (SUCCEEDED(hr))
        std::cout << "  50.0 - 12.5 = " << result << std::endl;
    
    // Test Multiply
    hr = pMath->Multiply(6.0, 7.0, &result);
    if (SUCCEEDED(hr))
        std::cout << "  6.0 * 7.0 = " << result << std::endl;
    
    // Test Divide
    hr = pMath->Divide(100.0, 4.0, &result);
    if (SUCCEEDED(hr))
        std::cout << "  100.0 / 4.0 = " << result << std::endl;
    
    // Test Value property
    std::cout << "\nTesting Value property:" << std::endl;
    hr = pMath->put_Value(42.0);
    if (SUCCEEDED(hr))
        std::cout << "  Set Value = 42.0" << std::endl;
    
    double value = 0;
    hr = pMath->get_Value(&value);
    if (SUCCEEDED(hr))
        std::cout << "  Get Value = " << value << std::endl;
    
    std::cout << "\nAll operations completed successfully!" << std::endl;
    
    pMath->Release();
    CoUninitialize();
    return 0;
}

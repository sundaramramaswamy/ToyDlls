#include <windows.h>
#include <oleauto.h> // for LoadTypeLib/RegisterTypeLib

#include "MathImpl.h"

LONG g_cObjects = 0;
LONG g_cLocks = 0;

extern HINSTANCE g_hInstance;

// Factory to create Math objects.
class ClassFactory : public IClassFactory
{
private:
    LONG m_refCount;

public:
    ClassFactory() : m_refCount(1) {}

    // Returns pointer to requested interface if supported.
    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject)
    {
        if (!ppvObject)
            return E_POINTER;

        *ppvObject = nullptr;

        if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IClassFactory))
        {
            *ppvObject = static_cast<IClassFactory*>(this);
            AddRef();
            return S_OK;
        }

        return E_NOINTERFACE;
    }

    // Increment reference count atomically.
    STDMETHOD_(ULONG, AddRef)()
    {
        return InterlockedIncrement(&m_refCount);
    }

    // Decrement reference count atomically.
    STDMETHOD_(ULONG, Release)()
    {
        LONG count = InterlockedDecrement(&m_refCount);
        if (count == 0)
        {
            delete this;
        }
        return count;
    }

    // Instantiate a Math object.
    STDMETHOD(CreateInstance)(IUnknown* pUnkOuter, REFIID riid, void** ppvObject)
    {
        if (pUnkOuter != nullptr)
            return CLASS_E_NOAGGREGATION;

        MathImpl* pMath = new MathImpl();
        if (!pMath)
            return E_OUTOFMEMORY;

        HRESULT hr = pMath->QueryInterface(riid, ppvObject);
        pMath->Release();
        return hr;
    }

    // Normally DLL (In-Proc or EXE if Out-of-Proc) stays loaded as long as
    // there are outstanding COM object references.  This allows clients to
    // stop server from unloading; manual management for speed.
    STDMETHOD(LockServer)(BOOL fLock)
    {
        if (fLock)
            InterlockedIncrement(&g_cLocks);
        else
            InterlockedDecrement(&g_cLocks);
        return S_OK;
    }
};

// When a client requests for an interface, COM loads the corresponding DLL
// and calls this function to get the class factory for the requested CLSID.
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    if (IsEqualCLSID(rclsid, CLSID_Math))
    {
        ClassFactory* pFactory = new ClassFactory();
        if (!pFactory)
            return E_OUTOFMEMORY;

        HRESULT hr = pFactory->QueryInterface(riid, ppv);
        pFactory->Release();
        return hr;
    }

    return CLASS_E_CLASSNOTAVAILABLE;
}

// Unload when there are no objects or locks.
STDAPI DllCanUnloadNow()
{
    return (g_cObjects == 0 && g_cLocks == 0) ? S_OK : S_FALSE;
}

// Setup registry entries for COM to load this DLL upon CLSID_Math requests.
// > regsvr32 ComMath.dll
STDAPI DllRegisterServer()
{
    wchar_t szDllPath[MAX_PATH];
    if (!GetModuleFileNameW(g_hInstance, szDllPath, MAX_PATH)) {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    // Register the type library
    ITypeLib* pTypeLib = nullptr;
    HRESULT hr = LoadTypeLibEx(szDllPath, REGKIND_REGISTER, &pTypeLib);
    if (pTypeLib)
        pTypeLib->Release();
    if (FAILED(hr))
        return hr;

    // Register the CLSID
    HKEY hKey = nullptr;
    LONG result = RegCreateKeyExW(HKEY_CLASSES_ROOT,
        L"CLSID\\{ECC3A324-8EB0-4B88-8D52-3A9281C0D0AD}",
        0, nullptr, 0, KEY_WRITE, nullptr, &hKey, nullptr);
    if (result == ERROR_SUCCESS)
    {
        RegSetValueExW(hKey, nullptr, 0, REG_SZ, (BYTE*)L"ComMath.Math", sizeof(L"ComMath.Math"));
        RegCloseKey(hKey);

        // Register InprocServer32
        result = RegCreateKeyExW(HKEY_CLASSES_ROOT,
            L"CLSID\\{ECC3A324-8EB0-4B88-8D52-3A9281C0D0AD}\\InprocServer32",
            0, nullptr, 0, KEY_WRITE, nullptr, &hKey, nullptr);
        if (result == ERROR_SUCCESS)
        {
            RegSetValueExW(hKey, nullptr, 0, REG_SZ, (BYTE*)szDllPath, (wcslen(szDllPath) + 1) * sizeof(wchar_t));
            RegSetValueExW(hKey, L"ThreadingModel", 0, REG_SZ, (BYTE*)L"Apartment", sizeof(L"Apartment"));
            RegCloseKey(hKey);
        }

        // Register ProgID
        result = RegCreateKeyExW(HKEY_CLASSES_ROOT,
            L"CLSID\\{ECC3A324-8EB0-4B88-8D52-3A9281C0D0AD}\\ProgID",
            0, nullptr, 0, KEY_WRITE, nullptr, &hKey, nullptr);
        if (result == ERROR_SUCCESS)
        {
            RegSetValueExW(hKey, nullptr, 0, REG_SZ, (BYTE*)L"ComMath.Math", sizeof(L"ComMath.Math"));
            RegCloseKey(hKey);
        }
    }

    // Register the ProgID
    result = RegCreateKeyExW(HKEY_CLASSES_ROOT, L"ComMath.Math",
        0, nullptr, 0, KEY_WRITE, nullptr, &hKey, nullptr);
    if (result == ERROR_SUCCESS)
    {
        RegSetValueExW(hKey, nullptr, 0, REG_SZ, (BYTE*)L"ComMath Math Component", sizeof(L"ComMath Math Component"));
        RegCloseKey(hKey);

        result = RegCreateKeyExW(HKEY_CLASSES_ROOT, L"ComMath.Math\\CLSID",
            0, nullptr, 0, KEY_WRITE, nullptr, &hKey, nullptr);
        if (result == ERROR_SUCCESS)
        {
            RegSetValueExW(hKey, nullptr, 0, REG_SZ, (BYTE*)L"{ECC3A324-8EB0-4B88-8D52-3A9281C0D0AD}", sizeof(L"{ECC3A324-8EB0-4B88-8D52-3A9281C0D0AD}"));
            RegCloseKey(hKey);
        }
    }

    return S_OK;
}

// Drop entries made by DllRegisterServer.
// > regsvr32 /u ComMath.dll
STDAPI DllUnregisterServer()
{
    // Unregister type library
    ITypeLib* pTypeLib = nullptr;
    wchar_t szDllPath[MAX_PATH];
    if (GetModuleFileNameW(g_hInstance, szDllPath, MAX_PATH))
    {
        if (SUCCEEDED(LoadTypeLibEx(szDllPath, REGKIND_NONE, &pTypeLib)))
        {
            TLIBATTR* pAttr = nullptr;
            if (SUCCEEDED(pTypeLib->GetLibAttr(&pAttr)))
            {
                UnRegisterTypeLib(pAttr->guid, pAttr->wMajorVerNum, pAttr->wMinorVerNum, pAttr->lcid, pAttr->syskind);
                pTypeLib->ReleaseTLibAttr(pAttr);
            }
            pTypeLib->Release();
        }
    }

    // Unregister ProgID
    RegDeleteTreeW(HKEY_CLASSES_ROOT, L"ComMath.Math");

    // Unregister CLSID
    RegDeleteTreeW(HKEY_CLASSES_ROOT, L"CLSID\\{ECC3A324-8EB0-4B88-8D52-3A9281C0D0AD}");

    return S_OK;
}

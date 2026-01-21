#include <windows.h>
#include "MathImpl.h"

LONG g_cObjects = 0;
LONG g_cLocks = 0;

class ClassFactory : public IClassFactory
{
private:
    LONG m_refCount;

public:
    ClassFactory() : m_refCount(1) {}

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

    STDMETHOD_(ULONG, AddRef)()
    {
        return InterlockedIncrement(&m_refCount);
    }

    STDMETHOD_(ULONG, Release)()
    {
        LONG count = InterlockedDecrement(&m_refCount);
        if (count == 0)
        {
            delete this;
        }
        return count;
    }

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

    STDMETHOD(LockServer)(BOOL fLock)
    {
        if (fLock)
            InterlockedIncrement(&g_cLocks);
        else
            InterlockedDecrement(&g_cLocks);
        return S_OK;
    }
};

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

STDAPI DllCanUnloadNow()
{
    return (g_cObjects == 0 && g_cLocks == 0) ? S_OK : S_FALSE;
}

STDAPI DllRegisterServer()   { return S_OK; }

STDAPI DllUnregisterServer() { return S_OK; }

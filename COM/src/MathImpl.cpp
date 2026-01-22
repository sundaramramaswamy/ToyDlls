#include "MathImpl.h"
#include <cmath>

extern LONG g_cObjects;
extern HINSTANCE g_hInstance;

MathImpl::MathImpl() : m_refCount(1), m_value(0.0), m_pTypeInfo(nullptr)
{
    InterlockedIncrement(&g_cObjects);

    // Load type library and get type info for IMath interface
    ITypeLib* pTypeLib = nullptr;
    wchar_t szDllPath[MAX_PATH];
    if (GetModuleFileNameW(g_hInstance, szDllPath, MAX_PATH))
    {
        if (SUCCEEDED(LoadTypeLibEx(szDllPath, REGKIND_NONE, &pTypeLib)))
        {
            pTypeLib->GetTypeInfoOfGuid(IID_IMath, &m_pTypeInfo);
            pTypeLib->Release();
        }
    }
}

MathImpl::~MathImpl()
{
    if (m_pTypeInfo)
        m_pTypeInfo->Release();
    InterlockedDecrement(&g_cObjects);
}

STDMETHODIMP MathImpl::QueryInterface(REFIID riid, void** ppvObject)
{
    if (!ppvObject)
        return E_POINTER;

    *ppvObject = nullptr;

    if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDispatch) || IsEqualIID(riid, IID_IMath))
    {
        *ppvObject = static_cast<IMath*>(this);
        AddRef();
        return S_OK;
    }

    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) MathImpl::AddRef()
{
    return InterlockedIncrement(&m_refCount);
}

STDMETHODIMP_(ULONG) MathImpl::Release()
{
    LONG count = InterlockedDecrement(&m_refCount);
    if (count == 0)
    {
        delete this;
    }
    return count;
}

STDMETHODIMP MathImpl::GetTypeInfoCount(UINT* pctinfo)
{
    if (!pctinfo)
        return E_POINTER;
    *pctinfo = (m_pTypeInfo != nullptr) ? 1 : 0;
    return S_OK;
}

STDMETHODIMP MathImpl::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo)
{
    if (!ppTInfo)
        return E_POINTER;
    *ppTInfo = nullptr;
    if (iTInfo != 0)
        return DISP_E_BADINDEX;
    if (!m_pTypeInfo)
        return E_NOTIMPL;
    m_pTypeInfo->AddRef();
    *ppTInfo = m_pTypeInfo;
    return S_OK;
}

STDMETHODIMP MathImpl::GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId)
{
    if (!m_pTypeInfo)
        return E_NOTIMPL;
    return m_pTypeInfo->GetIDsOfNames(rgszNames, cNames, rgDispId);
}

STDMETHODIMP MathImpl::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr)
{
    if (!m_pTypeInfo)
        return E_NOTIMPL;
    return m_pTypeInfo->Invoke(static_cast<IMath*>(this), dispIdMember, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
}

STDMETHODIMP MathImpl::get_Value(double* pVal)
{
    if (!pVal)
        return E_POINTER;
    
    *pVal = m_value;
    return S_OK;
}

STDMETHODIMP MathImpl::put_Value(double newVal)
{
    m_value = newVal;
    return S_OK;
}

STDMETHODIMP MathImpl::Add(double a, double b, double* result)
{
    if (!result)
        return E_POINTER;
    
    *result = a + b;
    return S_OK;
}

STDMETHODIMP MathImpl::Subtract(double a, double b, double* result)
{
    if (!result)
        return E_POINTER;
    
    *result = a - b;
    return S_OK;
}

STDMETHODIMP MathImpl::Multiply(double a, double b, double* result)
{
    if (!result)
        return E_POINTER;
    
    *result = a * b;
    return S_OK;
}

STDMETHODIMP MathImpl::Divide(double a, double b, double* result)
{
    if (!result)
        return E_POINTER;
    
    if (b == 0.0)
        return E_INVALIDARG;
    
    *result = a / b;
    return S_OK;
}

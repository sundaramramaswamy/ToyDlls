#pragma once

#include <windows.h>
#include <unknwn.h>
#include "ComMath.h"

class MathImpl : public IMath
{
private:
    LONG m_refCount;
    double m_value;

public:
    MathImpl();
    virtual ~MathImpl();

    // IUnknown methods
    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject);
    STDMETHOD_(ULONG, AddRef)();
    STDMETHOD_(ULONG, Release)();

    // IDispatch methods
    STDMETHOD(GetTypeInfoCount)(UINT* pctinfo);
    STDMETHOD(GetTypeInfo)(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo);
    STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId);
    STDMETHOD(Invoke)(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr);

    // IMath methods
    STDMETHOD(get_Value)(double* pVal);
    STDMETHOD(put_Value)(double newVal);
    STDMETHOD(Add)(double a, double b, double* result);
    STDMETHOD(Subtract)(double a, double b, double* result);
    STDMETHOD(Multiply)(double a, double b, double* result);
    STDMETHOD(Divide)(double a, double b, double* result);
};

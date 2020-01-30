#include "datetimewrapper.h"

DateTimeWrapper::DateTimeWrapper(INT64 ms)
{
    // The FILETIME structure is a 64-bit value
    // representing the number of 100-nanosecond intervals since January 1, 1601.
    // And in such case we need to conver ms to similiar value.
    _dateTime.UniversalTime = getCurrentTime() + (ms * 10000);
}

DateTimeWrapper::~DateTimeWrapper() {}

HRESULT STDMETHODCALLTYPE DateTimeWrapper::get_Value(DateTime *dateTime)
{
    *dateTime = _dateTime;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE DateTimeWrapper::QueryInterface(const IID &riid, void **ppvObject)
{
    if (ppvObject == nullptr) {
        return E_POINTER;
    }

    bool validQueryInterface = false;

    if (riid == __uuidof(IUnknown)) {
        *ppvObject = static_cast<IUnknown *>(this);
        validQueryInterface = true;
    } else if (riid == __uuidof(IReference<DateTime>)) {
        *ppvObject = this;
        validQueryInterface = true;
    }

    if (validQueryInterface) {
        AddRef();
    }

    return validQueryInterface ? S_OK : E_NOINTERFACE;
}

#pragma once

#include <windows.h>
#include <sal.h>
#include <psapi.h>
#include <strsafe.h>
#include <objbase.h>
#include <shobjidl.h>
#include <functiondiscoverykeys.h>
#include <guiddef.h>
#include <shlguid.h>

#include <wrl/client.h>
#include <wrl/implements.h>
#include <windows.ui.notifications.h>

#include "dynamic/base/noncopyable.h"

using namespace Microsoft::WRL;
using namespace ABI::Windows::Data::Xml::Dom;
using namespace ABI::Windows::Foundation;

/*
 * Wraps DateTime structure for proper construction and calculation
 * of visibility timeout for toasts in Windows Action Center.
 * More details about IReference here:
 * https://docs.microsoft.com/en-us/uwp/api/windows.foundation.ireference_t_
 * Since we are using it as base class, we need to implement needed interface methods.
 */
class DateTimeWrapper final : public IReference<DateTime>
{
    DISABLE_COPY(DateTimeWrapper)
public:
    // Construct DateTimeWrapper object based on some ms value. As result class
    // contains timestamp calculated by formula  = (DateTime then constructor was called + value in
    // ms)
    explicit DateTimeWrapper(INT64 ms = 0);
    ~DateTimeWrapper() = default;

    HRESULT STDMETHODCALLTYPE get_Value(DateTime *dateTime);

    /*
     * Implementation of COM interface
     * Quick start and details about COM technology:
     * https://docs.microsoft.com/en-us/windows/win32/com/component-object-model--com--portal
     */

    // IUknown interface implementation

    // Mechanism to find out whether the object supports another specific interface and,
    // if so, to get a pointer to it.
    HRESULT STDMETHODCALLTYPE QueryInterface(const IID &riid, void **ppvObject);

    // Increments the reference count for an interface pointer to a COM object.
    ULONG STDMETHODCALLTYPE AddRef() { return ++_refCount; }

    // Decrements point reference count, and deletes self if reference count reduces to zero.
    ULONG STDMETHODCALLTYPE Release()
    {
        ULONG postReleaseCount = --_refCount;
        if (postReleaseCount == 0) {
            /*
             * Last reference released. When the reference count on an object reaches zero,
             * Release must cause the interface pointer to free itself. When the released pointer
             * is the only (formerly) outstanding reference to an object, the implementation must
             * free the object. More information:
             * https://docs.microsoft.com/en-us/windows/win32/api/unknwn/nf-unknwn-iunknown-release
             */
            delete this;
        }

        return postReleaseCount;
    }

    // IInspectable interface implementation
    HRESULT STDMETHODCALLTYPE GetIids(ULONG *, IID **) { return E_NOTIMPL; }

    HRESULT STDMETHODCALLTYPE GetRuntimeClassName(HSTRING *) { return E_NOTIMPL; }

    HRESULT STDMETHODCALLTYPE GetTrustLevel(TrustLevel *) { return E_NOTIMPL; }

private:
    // Reference counter
    ULONG _refCount = 0;

    // DateTime value storage
    DateTime _wrappedValue = { 0 };

    // Help to calculate current timestamp.
    // The FILETIME structure is a 64-bit value
    // representing the number of 100-nanosecond intervals since January 1, 1601.
    INT64 getCurrentTime()
    {
        FILETIME now;
        GetSystemTimeAsFileTime(&now);
        return ((((INT64)now.dwHighDateTime) << 32) | now.dwLowDateTime);
    }
};

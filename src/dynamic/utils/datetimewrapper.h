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

// Wraps DateTime structure for proper construction and
// calculation of toast showing offset
class DateTimeWrapper final : public IReference<DateTime>
{
	DISABLE_COPY(DateTimeWrapper)
public:
	explicit DateTimeWrapper(INT64 ms = 0);
	~DateTimeWrapper();

	// Implementation of COM interface
	HRESULT STDMETHODCALLTYPE get_Value(DateTime *dateTime);

	HRESULT STDMETHODCALLTYPE QueryInterface(const IID &riid, void **ppvObject);

	ULONG STDMETHODCALLTYPE AddRef() {
		return ++_refCount;
	}

    // decrements point reference count, and deletes self if reference count reduces to zero
	ULONG STDMETHODCALLTYPE Release() {
		ULONG newRef = --_refCount;
		if (!newRef)
            // MS Docs: When the reference count on an object reaches zero, Release must cause the interface pointer to free itself. 
            // When the released pointer is the only (formerly) outstanding reference to an object, the implementation must free the object.
			delete this;
          }

		return newRef;
	}

	HRESULT STDMETHODCALLTYPE GetIids(ULONG *, IID **) { return E_NOTIMPL; }

	HRESULT STDMETHODCALLTYPE GetRuntimeClassName(HSTRING *) { return E_NOTIMPL; }

	HRESULT STDMETHODCALLTYPE GetTrustLevel(TrustLevel *) { return E_NOTIMPL; }

private:
	// Reference counter
	ULONG _refCount = 0;

	// DateTime value storage
	DateTime _dateTime = { 0 };

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

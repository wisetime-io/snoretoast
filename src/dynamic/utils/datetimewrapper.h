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

class DateTimeWrapper final : public IReference<DateTime>
{
	DISABLE_COPY(DateTimeWrapper)
public:
	explicit DateTimeWrapper(INT64 ms = 0);

	~DateTimeWrapper();

	HRESULT STDMETHODCALLTYPE get_Value(DateTime *dateTime);

	HRESULT STDMETHODCALLTYPE QueryInterface(const IID &riid, void **ppvObject);

	ULONG STDMETHODCALLTYPE AddRef() {
		return ++_refCount;
	}

	ULONG STDMETHODCALLTYPE Release() {
		ULONG newRef = --_refCount;
		if (!newRef)
			delete this;

		return newRef;
	}

	HRESULT STDMETHODCALLTYPE GetIids(ULONG *, IID **) { return E_NOTIMPL; }

	HRESULT STDMETHODCALLTYPE GetRuntimeClassName(HSTRING *) { return E_NOTIMPL; }

	HRESULT STDMETHODCALLTYPE GetTrustLevel(TrustLevel *) { return E_NOTIMPL; }

private:
	ULONG _refCount = 0;
	DateTime _dateTime = { 0 };

	INT64 getCurrentTime()
	{
		FILETIME now;
		GetSystemTimeAsFileTime(&now);
		return ((((INT64)now.dwHighDateTime) << 32) | now.dwLowDateTime);
	}
};

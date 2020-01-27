#include "dynamic/winrt-base.h"
#include "dynamic/api/combaseapi.h"
#include "dynamic/api/rterrorapi.h"

namespace {
static const ComBaseApi *comBaseApi = nullptr;
static const RtErrorApi *rtErrorApi = nullptr;
}

namespace winrt {

bool isCompatible()
{
    bool hasComApi = comBaseApi == nullptr ? false : comBaseApi->isAvailable();
    bool hasErrorApi = rtErrorApi == nullptr ? false : rtErrorApi->isAvailable();

	return hasComApi && hasErrorApi;
}

bool LoadApi()
{
    if (rtErrorApi == nullptr)
        rtErrorApi = new RtErrorApi();

    if (comBaseApi == nullptr)
	    comBaseApi = new ComBaseApi();

	return isCompatible();
}

void UnloadApi() 
{
	if(comBaseApi != nullptr)
		delete comBaseApi;

	if (rtErrorApi != nullptr)
		delete rtErrorApi;
}

HRESULT RoInitialize(RO_INIT_TYPE init_type)
{
    if (!isCompatible())
        return E_FAIL;

    return comBaseApi->RoInitialize(init_type);
}

void RoUninitialize()
{
    if (!isCompatible())
        return;

    return comBaseApi->RoUninitialize();
}

HRESULT RoGetActivationFactory(HSTRING class_id, const IID &iid, void **out_factory)
{
    if (!isCompatible())
        return E_FAIL;

    return comBaseApi->RoGetActivationFactory(class_id, iid, out_factory);
}

HRESULT RoActivateInstance(HSTRING class_id, IInspectable **instance)
{
    if (!isCompatible())
        return E_FAIL;

	return comBaseApi->RoActivateInstance(class_id, instance);
}

HRESULT WindowsCreateStringReference(PCWSTR sourceString, UINT32 length,
                                     HSTRING_HEADER *hstringHeader, HSTRING *string)
{
    if (!isCompatible())
        return E_FAIL;

	return comBaseApi->WindowsCreateStringReference(sourceString, length, hstringHeader, string);
}

HRESULT WindowsDeleteString(HSTRING string)
{
    if (!isCompatible())
        return E_FAIL;

	return comBaseApi->WindowsDeleteString(string);
}
};

STDAPI_(BOOL)
RoOriginateError(_In_ HRESULT error, _In_opt_ HSTRING message)
{
    if (!winrt::isCompatible())
        return E_FAIL;

	return rtErrorApi->RoOriginateError(error, message);
}

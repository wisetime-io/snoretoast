#pragma once

#include <sdkddkver.h>

// Windows Header Files:
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

#include <filesystem>
#include <string>
#include <vector>

using namespace Microsoft::WRL;
using namespace ABI::Windows::Data::Xml::Dom;

namespace winrt {

bool LoadApi();
void UnloadApi();

bool isCompatible();

HRESULT RoInitialize(RO_INIT_TYPE init_type);
void RoUninitialize();

HRESULT RoGetActivationFactory(HSTRING class_id, const IID &iid, void **out_factory);

HRESULT RoActivateInstance(HSTRING class_id, IInspectable **instance);

HRESULT WindowsCreateStringReference(PCWSTR sourceString, UINT32 length,
                                     HSTRING_HEADER *hstringHeader, HSTRING *string);

HRESULT WindowsDeleteString(HSTRING string);
};

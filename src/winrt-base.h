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

namespace dllimporter {

// TODO: replace with templates and use single function
template<typename Function>
HRESULT loadFunctionFromCom(LPCSTR name, Function &func)
{
    static HMODULE const library = ::LoadLibrary(L"combase.dll");
    if (!library) {
        return E_INVALIDARG;
    }
    func = reinterpret_cast<Function>(GetProcAddress(library, name));
    return (func != nullptr) ? S_OK : E_FAIL;
}

FARPROC LoadComBaseFunction(const char *function_name);

decltype(&::RoInitialize) GetRoInitializeFunction();

decltype(&::RoUninitialize) GetRoUninitializeFunction();

decltype(&::RoActivateInstance) GetRoActivateInstanceFunction();

decltype(&::RoGetActivationFactory) GetRoGetActivationFactoryFunction();

}; // namespace dllimporter

typedef HRESULT(FAR STDAPICALLTYPE *f_WindowsCreateStringReference)(
        _In_reads_opt_(length + 1) PCWSTR sourceString, UINT32 length,
        _Out_ HSTRING_HEADER *hstringHeader,
        _Outptr_result_maybenull_ _Result_nullonfailure_ HSTRING *string);

typedef PCWSTR(FAR STDAPICALLTYPE *f_WindowsGetStringRawBuffer)(_In_ HSTRING string,
                                                                _Out_ UINT32 *length);

typedef HRESULT(FAR STDAPICALLTYPE *f_WindowsDeleteString)(_In_opt_ HSTRING string);

namespace winrt {
static f_WindowsCreateStringReference WindowsCreateStringReference;
static f_WindowsGetStringRawBuffer WindowsGetStringRawBuffer;
static f_WindowsDeleteString WindowsDeleteString;

bool LoadApi();

bool LoadStringApi();

HRESULT RoInitialize(RO_INIT_TYPE init_type);
void RoUninitialize();

HRESULT RoGetActivationFactory(HSTRING class_id, const IID &iid, void **out_factory);

HRESULT RoActivateInstance(HSTRING class_id, IInspectable **instance);
};

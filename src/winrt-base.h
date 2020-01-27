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

class ProcPtr
{
public:
    explicit ProcPtr(FARPROC ptr) : _ptr(ptr) {}

    template<typename T, typename = std::enable_if_t<std::is_function_v<T>>>
    operator T *() const
    {
        return reinterpret_cast<T *>(_ptr);
    }

    bool isAvailable() const { return _ptr != nullptr; }

private:
    FARPROC _ptr;
};

class DllLoader
{
public:
    explicit DllLoader(LPCSTR filename) { _module = LoadLibraryA(filename); }

    ~DllLoader() { FreeLibrary(_module); }

    ProcPtr operator[](LPCSTR name) const
    {
        if (_module != nullptr)
            return ProcPtr(GetProcAddress(_module, name));

        return ProcPtr(nullptr);
    }

private:
    HMODULE _module;
};

class DynamicApiLoader
{
    DynamicApiLoader(DynamicApiLoader &) = delete;
    DynamicApiLoader &operator=(DynamicApiLoader &) = delete;

protected:
    DllLoader _dllLoader;

    DynamicApiLoader(LPCSTR filename) : _dllLoader(filename) {}

    ProcPtr resolve(LPCSTR name) const { return _dllLoader[name]; }

public:
    virtual ~DynamicApiLoader() {}

    virtual bool isAvailable() const = 0;
};

class RtErrorApi final : public DynamicApiLoader
{
public:
    RtErrorApi() : DynamicApiLoader("api-ms-win-core-winrt-error-l1-1-0.dll")
    {
        RoOriginateError = DynamicApiLoader::resolve("RoOriginateError");
    }

    decltype(RoOriginateError) *RoOriginateError = nullptr;

    bool isAvailable() const override
    {
        return this->RoOriginateError != nullptr;
    }
};

class ComBaseApi final : public DynamicApiLoader
{
public:
    ComBaseApi() : DynamicApiLoader("combase.dll")
    {
        RoInitialize = DynamicApiLoader::resolve("RoInitialize");
        RoUninitialize = DynamicApiLoader::resolve("RoUninitialize");

        RoActivateInstance = DynamicApiLoader::resolve("RoActivateInstance");
        RoGetActivationFactory = DynamicApiLoader::resolve("RoGetActivationFactory");
        
		WindowsCreateStringReference = DynamicApiLoader::resolve("WindowsCreateStringReference");
        WindowsDeleteString = DynamicApiLoader::resolve("WindowsDeleteString");
    }

    bool isAvailable() const override
    {
        return this->RoInitialize != nullptr && this->RoUninitialize != nullptr
                && this->RoActivateInstance != nullptr && this->RoGetActivationFactory != nullptr
                && this->WindowsCreateStringReference != nullptr
                && this->WindowsDeleteString != nullptr;
    }

    decltype(RoInitialize) *RoInitialize = nullptr;
    decltype(RoUninitialize) *RoUninitialize = nullptr;

    decltype(RoActivateInstance) *RoActivateInstance = nullptr;
    decltype(RoGetActivationFactory) *RoGetActivationFactory = nullptr;

    decltype(WindowsCreateStringReference) *WindowsCreateStringReference = nullptr;
    decltype(WindowsDeleteString) *WindowsDeleteString = nullptr;
};

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

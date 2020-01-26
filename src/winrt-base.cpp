#include "winrt-base.h"

namespace dllimporter {

FARPROC LoadComBaseFunction(const char *function_name)
{
    // TODO: add free library
    static HMODULE const handle = ::LoadLibrary(L"combase.dll");
    return handle ? ::GetProcAddress(handle, function_name) : nullptr;
}

FARPROC LoadRtErrorFunction(const char *function_name)
{
    // TODO: add free library
    static HMODULE const handle = ::LoadLibrary(L"api-ms-win-core-winrt-error-l1-1-0.dll");
    return handle ? ::GetProcAddress(handle, function_name) : nullptr;
}

decltype(&::RoInitialize) GetRoInitializeFunction()
{
    static decltype(&::RoInitialize) const function =
            reinterpret_cast<decltype(&::RoInitialize)>(LoadComBaseFunction("RoInitialize"));
    return function;
}

decltype(&::RoUninitialize) GetRoUninitializeFunction()
{
    static decltype(&::RoUninitialize) const function =
            reinterpret_cast<decltype(&::RoUninitialize)>(LoadComBaseFunction("RoUninitialize"));
    return function;
}

decltype(&::RoActivateInstance) GetRoActivateInstanceFunction()
{
    static decltype(&::RoActivateInstance) const function =
            reinterpret_cast<decltype(&::RoActivateInstance)>(
                    LoadComBaseFunction("RoActivateInstance"));
    return function;
}

decltype(&::RoGetActivationFactory) GetRoGetActivationFactoryFunction()
{
    static decltype(&::RoGetActivationFactory) const function =
            reinterpret_cast<decltype(&::RoGetActivationFactory)>(
                    LoadComBaseFunction("RoGetActivationFactory"));
    return function;
}

decltype(&::RoOriginateError) GetRoOriginateErrorFunction()
{
    static decltype(&::RoOriginateError) const function =
            reinterpret_cast<decltype(&::RoOriginateError)>(
                    LoadRtErrorFunction("RoOriginateError"));
    return function;
}

decltype(&::WindowsCreateStringReference) GetWindowsCreateStringReference()
{
    static decltype(&::WindowsCreateStringReference) const function =
            reinterpret_cast<decltype(&::WindowsCreateStringReference)>(
                    LoadComBaseFunction("WindowsCreateStringReference"));
    return function;
}

decltype(&::WindowsDeleteString) GetWindowsDeleteString()
{
    static decltype(&::WindowsDeleteString) const function =
            reinterpret_cast<decltype(&::WindowsDeleteString)>(
                    LoadComBaseFunction("WindowsDeleteString"));
    return function;
}

}; // namespace dllimporter

namespace winrt {

bool LoadApi()
{
    // TODO(finnur): Add AssertIOAllowed once crbug.com/770193 is fixed.
    return dllimporter::GetRoInitializeFunction() && dllimporter::GetRoUninitializeFunction()
            && dllimporter::GetRoActivateInstanceFunction()
            && dllimporter::GetRoGetActivationFactoryFunction()
            && dllimporter::GetRoOriginateErrorFunction()
            && dllimporter::GetWindowsCreateStringReference()
			&& dllimporter::GetWindowsDeleteString();
}

HRESULT RoInitialize(RO_INIT_TYPE init_type)
{
    auto ro_initialize_func = dllimporter::GetRoInitializeFunction();
    if (!ro_initialize_func)
        return E_FAIL;
    return ro_initialize_func(init_type);
}

void RoUninitialize()
{
    auto ro_uninitialize_func = dllimporter::GetRoUninitializeFunction();
    if (ro_uninitialize_func)
        ro_uninitialize_func();
}

HRESULT RoGetActivationFactory(HSTRING class_id, const IID &iid, void **out_factory)
{
    auto get_factory_func = dllimporter::GetRoGetActivationFactoryFunction();
    if (!get_factory_func)
        return E_FAIL;
    return get_factory_func(class_id, iid, out_factory);
}

HRESULT RoActivateInstance(HSTRING class_id, IInspectable **instance)
{
    auto activate_instance_func = dllimporter::GetRoActivateInstanceFunction();
    if (!activate_instance_func)
        return E_FAIL;
    return activate_instance_func(class_id, instance);
}

HRESULT WindowsCreateStringReference(PCWSTR sourceString, UINT32 length,
                                     HSTRING_HEADER *hstringHeader, HSTRING *string)
{
    auto activate_instance_func = dllimporter::GetWindowsCreateStringReference();
    if (!activate_instance_func)
        return E_FAIL;
    
	return activate_instance_func(sourceString, length, hstringHeader, string);
}

HRESULT WindowsDeleteString(HSTRING string)
{
    auto activate_instance_func = dllimporter::GetWindowsDeleteString();
    if (!activate_instance_func)
        return E_FAIL;

    return activate_instance_func(string);
}

};

STDAPI_(BOOL)
RoOriginateError(
    _In_ HRESULT error,
    _In_opt_ HSTRING message
    )
{
    auto activate_instance_func = dllimporter::GetRoOriginateErrorFunction();
    if (!activate_instance_func)
        return E_FAIL;
    return activate_instance_func(error, message);
}




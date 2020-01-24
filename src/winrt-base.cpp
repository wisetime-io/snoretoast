#include "winrt-base.h"

namespace dllimporter {

FARPROC LoadComBaseFunction(const char *function_name)
{
    // TODO: add free library
    static HMODULE const handle = ::LoadLibrary(L"combase.dll");
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

}; // namespace dllimporter

namespace winrt {

bool LoadApi()
{
    // TODO(finnur): Add AssertIOAllowed once crbug.com/770193 is fixed.
    return dllimporter::GetRoInitializeFunction() && dllimporter::GetRoUninitializeFunction()
            && dllimporter::GetRoActivateInstanceFunction()
            && dllimporter::GetRoGetActivationFactoryFunction();
}

bool LoadStringApi()
{
    const bool succeded = SUCCEEDED(dllimporter::loadFunctionFromCom("WindowsCreateStringReference",
                                                                     WindowsCreateStringReference))
            && SUCCEEDED(dllimporter::loadFunctionFromCom("WindowsGetStringRawBuffer",
                                                          WindowsGetStringRawBuffer))
            && SUCCEEDED(dllimporter::loadFunctionFromCom("WindowsDeleteString",
                                                          WindowsDeleteString));
    return succeded;
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
};

#include "dynamic/api/combaseapi.h"

 ComBaseApi::ComBaseApi() : DynamicApiLoader("combase.dll")
{
    RoInitialize = DynamicApiLoader::resolve("RoInitialize");
    RoUninitialize = DynamicApiLoader::resolve("RoUninitialize");

    RoActivateInstance = DynamicApiLoader::resolve("RoActivateInstance");
    RoGetActivationFactory = DynamicApiLoader::resolve("RoGetActivationFactory");

    WindowsCreateStringReference = DynamicApiLoader::resolve("WindowsCreateStringReference");
    WindowsDeleteString = DynamicApiLoader::resolve("WindowsDeleteString");
}

bool ComBaseApi::isAvailable() const
{
    return this->RoInitialize != nullptr && this->RoUninitialize != nullptr
            && this->RoActivateInstance != nullptr && this->RoGetActivationFactory != nullptr
            && this->WindowsCreateStringReference != nullptr
            && this->WindowsDeleteString != nullptr;
}

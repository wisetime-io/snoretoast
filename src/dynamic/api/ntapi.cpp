#include "dynamic/api/ntapi.h"

NtApi::NtApi() : DynamicApiLoader("ntdll.dll")
{
    RtlGetVersion = DynamicApiLoader::resolve("RtlGetVersion");
}

bool NtApi::isAvailable() const
{
    return this->RtlGetVersion != nullptr;
}
#include "dynamic/api/rterrorapi.h"

RtErrorApi::RtErrorApi() : DynamicApiLoader("api-ms-win-core-winrt-error-l1-1-0.dll")
{
    RoOriginateError = DynamicApiLoader::resolve("RoOriginateError");
}

bool RtErrorApi::isAvailable() const
{
    return this->RoOriginateError != nullptr;
}

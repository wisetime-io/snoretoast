#pragma once

#include "dynamic/winrt-base.h"
#include "dynamic/base/dynamicapiloader.h"

class RtErrorApi final : public DynamicApiLoader
{
public:
    explicit RtErrorApi();
    bool isAvailable() const override;

	decltype(RoOriginateError) *RoOriginateError = nullptr;
};

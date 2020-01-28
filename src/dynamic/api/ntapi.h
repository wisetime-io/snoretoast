#pragma once
#include <windows.h>

#include "dynamic/winrt-base.h"
#include "dynamic/base/dynamicapiloader.h"

typedef LONG NTSTATUS, *PNTSTATUS;
NTSTATUS WINAPI RtlGetVersion(PRTL_OSVERSIONINFOW version);

#define STATUS_SUCCESS (0x00000000)

class NtApi final : public DynamicApiLoader
{
public:
    explicit NtApi();
    bool isAvailable() const override;

    decltype(RtlGetVersion) *RtlGetVersion = nullptr;
};

#include "dynamic/winrt-base.h"
#include "dynamic/base/dynamicapiloader.h"

class ComBaseApi final : public DynamicApiLoader
{
public:
    explicit ComBaseApi();
    bool isAvailable() const override;

    decltype(RoInitialize) *RoInitialize = nullptr;
    decltype(RoUninitialize) *RoUninitialize = nullptr;

    decltype(RoActivateInstance) *RoActivateInstance = nullptr;
    decltype(RoGetActivationFactory) *RoGetActivationFactory = nullptr;

    decltype(WindowsCreateStringReference) *WindowsCreateStringReference = nullptr;
    decltype(WindowsDeleteString) *WindowsDeleteString = nullptr;
};

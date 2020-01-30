#pragma once

#include "dynamic/base/noncopyable.h"

class ActivationCallback
{
    DISABLE_COPY(ActivationCallback)
public:
    ActivationCallback() = default;
    virtual ~ActivationCallback() {}

    virtual HRESULT onActivate(const std::wstring &appUserModelId, const std::wstring &invokedArgs,
                               const std::wstring &msg) = 0;
};

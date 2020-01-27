#pragma once

#include "dynamic/base/noncopyable.h"
#include "dynamic/base/dllloader.h"

class DynamicApiLoader
{
    DISABLE_COPY(DynamicApiLoader)
protected:
    DllLoader _dllLoader;

    DynamicApiLoader(LPCSTR filename) : _dllLoader(filename) {}

    DllProcPtr resolve(LPCSTR name) const { return _dllLoader[name]; }

public:
    virtual ~DynamicApiLoader() {}

    virtual bool isAvailable() const = 0;
};

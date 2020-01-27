#include <windows.h>
#include <type_traits>

#pragma once

class DllProcPtr final
{
public:
    explicit DllProcPtr(FARPROC ptr) : _ptr(ptr) {}

    template<typename T, typename = std::enable_if_t<std::is_function_v<T>>>
    operator T *() const
    {
        return reinterpret_cast<T *>(_ptr);
    }
private:
    FARPROC _ptr;
};

class DllLoader final
{
public:
    explicit DllLoader(LPCSTR filename) { _module = LoadLibraryA(filename); }

    ~DllLoader() { FreeLibrary(_module); }

    DllProcPtr operator[](LPCSTR name) const
    {
        return _module != nullptr ? DllProcPtr(GetProcAddress(_module, name)) : DllProcPtr(nullptr);
    }

private:
    HMODULE _module;
};

#pragma once

#include "dynamic/winrt-base.h"
#include "dynamic/base/noncopyable.h"

class StringWrapper final
{
	DISABLE_COPY(StringWrapper)
public:
    explicit StringWrapper(_In_reads_(length) PCWSTR stringRef, _In_ UINT32 length) noexcept;
    explicit StringWrapper(_In_ const std::wstring &stringRef) noexcept;
    
    ~StringWrapper();

    inline HSTRING Get() const noexcept { return _hstring; }

private:
    HSTRING _hstring;
    HSTRING_HEADER _header;
};

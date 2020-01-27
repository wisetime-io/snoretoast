#include "dynamic/utils/stringwrapper.h"

StringWrapper::StringWrapper(_In_reads_(length) PCWSTR stringRef, _In_ UINT32 length) noexcept
{
	HRESULT hr = winrt::WindowsCreateStringReference(stringRef, length, &_header, &_hstring);
	if (!SUCCEEDED(hr)) {
		RaiseException(static_cast<DWORD>(STATUS_INVALID_PARAMETER), EXCEPTION_NONCONTINUABLE,
					   0, nullptr);
	}
}

StringWrapper::StringWrapper(_In_ const std::wstring &stringRef) noexcept
{
	const wchar_t *data = stringRef.c_str();
	UINT32 len = static_cast<UINT32>(stringRef.length());

	HRESULT hr = winrt::WindowsCreateStringReference(data, len,
													 &_header, &_hstring);
	if (FAILED(hr)) {
		RaiseException(static_cast<DWORD>(STATUS_INVALID_PARAMETER), EXCEPTION_NONCONTINUABLE,
					   0, nullptr);
	}
}

StringWrapper::~StringWrapper() { 
	winrt::WindowsDeleteString(_hstring);
}

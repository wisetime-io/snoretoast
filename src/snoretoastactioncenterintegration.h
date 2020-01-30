/*
    SnoreToast is capable to invoke Windows 8 toast notifications.
    Copyright (C) 2019  Hannah von Reth <vonreth@kde.org>

    SnoreToast is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SnoreToast is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with SnoreToast.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <algorithm>
#include <ntverp.h>
#include <sstream>
#include <wrl.h>
#include <mutex>

#include "activationcallback.h"
#include "dynamic/base/noncopyable.h"

#define ST_WSTRINGIFY(X) L##X
#define ST_STRINGIFY(X) ST_WSTRINGIFY(X)

class ActivationCallbackWrapper final
{
    DISABLE_COPY(ActivationCallbackWrapper)
public:
    ActivationCallbackWrapper() = default;

    void set(ActivationCallback *callback)
    {
        std::lock_guard<std::mutex> lock(_callbackMutex);
        _callback = callback;
    }

	void remove() { set(nullptr); }

    HRESULT invokeOnActivate(const std::wstring &appUserModelId, const std::wstring &invokedArgs,
                             const std::wstring &msg)
    {
        std::lock_guard<std::mutex> lock(_callbackMutex);

        return (_callback != nullptr) ? _callback->onActivate(appUserModelId, invokedArgs, msg)
                                      : S_OK;
    }

private:
    std::mutex _callbackMutex;
    ActivationCallback *_callback = nullptr;
};

typedef struct NOTIFICATION_USER_INPUT_DATA
{
    LPCWSTR Key;
    LPCWSTR Value;
} NOTIFICATION_USER_INPUT_DATA;

MIDL_INTERFACE("53E31837-6600-4A81-9395-75CFFE746F94")
INotificationActivationCallback : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE Activate(
            __RPC__in_string LPCWSTR appUserModelId, __RPC__in_opt_string LPCWSTR invokedArgs,
            __RPC__in_ecount_full_opt(count) const NOTIFICATION_USER_INPUT_DATA *data,
            ULONG count) = 0;
};

// The COM server which implements the callback notification from Action Center
class DECLSPEC_UUID(SNORETOAST_CALLBACK_GUID) SnoreToastActionCenterIntegration
    : public Microsoft::WRL::RuntimeClass<
              Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>,
              INotificationActivationCallback>
{
public:
    static ActivationCallbackWrapper callback;
    static std::wstring uuid() { return ST_STRINGIFY(SNORETOAST_CALLBACK_GUID); }

    SnoreToastActionCenterIntegration() {}
    virtual HRESULT STDMETHODCALLTYPE Activate(__RPC__in_string LPCWSTR appUserModelId,
                                               __RPC__in_opt_string LPCWSTR invokedArgs,
                                               __RPC__in_ecount_full_opt(count)
                                                       const NOTIFICATION_USER_INPUT_DATA *data,
                                               ULONG count) override
    {
        if (invokedArgs == nullptr) {
            return S_OK;
        }

        std::wstringstream msg;
        for (ULONG i = 0; i < count; ++i) {
            std::wstring tmp = data[i].Value;
            // printing \r to stdcout is kind of problematic :D
            std::replace(tmp.begin(), tmp.end(), L'\r', L'\n');
            msg << tmp;
        }

        return SnoreToastActionCenterIntegration::callback.invokeOnActivate(
                appUserModelId, invokedArgs, msg.str());
    }
};

ActivationCallbackWrapper SnoreToastActionCenterIntegration::callback;

CoCreatableClass(SnoreToastActionCenterIntegration);

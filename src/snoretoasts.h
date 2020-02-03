/*
        SnoreToast is capable to invoke Windows 8 toast notifications.
        Copyright (C) 2013-2019  Hannah von Reth <vonreth@kde.org>

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

#include "snoretoastactions.h"
#include "libsnoretoast_export.h"

#include <sdkddkver.h>

// Windows Header Files:
#include <windows.h>
#include <sal.h>
#include <psapi.h>
#include <strsafe.h>
#include <objbase.h>
#include <shobjidl.h>
#include <functiondiscoverykeys.h>
#include <guiddef.h>
#include <shlguid.h>

#include <wrl/client.h>
#include <wrl/implements.h>
#include <windows.ui.notifications.h>

#include <filesystem>
#include <string>
#include <vector>
#include <functional>

#include "dynamic/winrt-base.h"
#include "dynamic/base/noncopyable.h"
#include "dynamic/utils/datetimewrapper.h"
#include "activationcallback.h"

using namespace Microsoft::WRL;
using namespace ABI::Windows::Data::Xml::Dom;
using namespace ABI::Windows::Foundation;

enum class Duration {
    Short, // default 7s
    Long // 25s
};

class LIBSNORETOAST_EXPORT SnoreToastsActivationCallback : public ActivationCallback
{
    DISABLE_COPY(SnoreToastsActivationCallback)
public:
    SnoreToastsActivationCallback() = default;

    void setOnToastActivate(std::function<void(const std::wstring &)> onToastActivate)
    {
        _onToastActivate = onToastActivate;
    }

private:
    std::function<void(const std::wstring &)> _onToastActivate;

    HRESULT onActivate(const std::wstring &appUserModelId, const std::wstring &invokedArgs,
                       const std::wstring &msg) override;
};

class LIBSNORETOAST_EXPORT SnoreToasts
{
public:
    static bool supportsModernFeatures();
    static std::wstring version();

    SnoreToasts(const std::wstring &appID);
    ~SnoreToasts();

	/*
	 * Display created toast with:
	 * - title
	 * - body
	 * - path to custom image
	 * - some time to live in action center (optional). Will be default in case of 0 or less.
	*/
    HRESULT displayToast(const std::wstring &title, const std::wstring &body,
                         const std::filesystem::path &image, INT64 toastActionCenterTtlSec = 0);
    bool closeNotification();

    void setSound(const std::wstring &soundFile);
    void setSilent(bool silent);
    void setId(const std::wstring &id);
    std::wstring id() const;

    void setButtons(const std::wstring &buttons);
    void setTextBoxEnabled(bool textBoxEnabled);

    void setPayload(const std::wstring &payload);

    std::filesystem::path application() const;
    void setApplication(const std::filesystem::path &application);

    Duration duration() const;
    void setDuration(Duration duration);

    std::wstring formatAction(const SnoreToastActions::Actions &action,
                              const std::vector<std::pair<std::wstring_view, std::wstring_view>>
                                      &extraData = {}) const;

private:
    HRESULT createToast(INT64 toastActionCenterTtlSec);
    HRESULT setImage();
    HRESULT setSound();
    HRESULT setTextValues();
    HRESULT setButtons(ComPtr<IXmlNode> root);
    HRESULT setTextBox(ComPtr<IXmlNode> root);
    HRESULT setNodeValueString(const HSTRING &onputString,
                               ABI::Windows::Data::Xml::Dom::IXmlNode *node);
    HRESULT addAttribute(const std::wstring &name,
                         ABI::Windows::Data::Xml::Dom::IXmlNamedNodeMap *attributeMap);
    HRESULT addAttribute(const std::wstring &name,
                         ABI::Windows::Data::Xml::Dom::IXmlNamedNodeMap *attributeMap,
                         const std::wstring &value);
    HRESULT createNewActionButton(ComPtr<IXmlNode> actionsNode, const std::wstring &value);

    friend class SnoreToastsPrivate;
    SnoreToastsPrivate *d;
};

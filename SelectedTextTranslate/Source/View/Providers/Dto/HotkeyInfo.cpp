#include "View\Providers\Dto\HotkeyInfo.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

HotkeyInfo::HotkeyInfo()
{
    this->windowHandle = nullptr;
    this->pressedCallback = function<void()>();
    this->hotkeyId = 0;
    this->modifiers = 0;
    this->virtualCode = 0;
    this->isRegistered = false;
}

HotkeyInfo::HotkeyInfo(HWND windowHandle, function<void()> pressedCallback, int hotkeyId, UINT modifiers, UINT virtualCode)
{
    this->windowHandle = windowHandle;
    this->pressedCallback = pressedCallback;
    this->hotkeyId = hotkeyId;
    this->modifiers = modifiers;
    this->virtualCode = virtualCode;
    this->isRegistered = false;
}

HotkeyInfo::HotkeyInfo(HWND windowHandle, function<void()> pressedCallback, int hotkeyId, int hotkey)
    :HotkeyInfo(windowHandle, pressedCallback, hotkeyId, HIBYTE(LOWORD(hotkey)), LOBYTE(LOWORD(hotkey)))
{
}

HWND HotkeyInfo::GetWindowHandle() const
{
    return windowHandle;
}

function<void()> HotkeyInfo::GetPressedCallback() const
{
    return pressedCallback;
}

int HotkeyInfo::GetHotkeyId() const
{
    return hotkeyId;
}

UINT HotkeyInfo::GetModifiers() const
{
    return modifiers;
}

UINT HotkeyInfo::GetVirtualCode() const
{
    return virtualCode;
}

void HotkeyInfo::Register()
{
    if(isRegistered)
    {
        return;
    }

    AssertWinApiResult(RegisterHotKey(
        GetWindowHandle(),
        GetHotkeyId(),
        GetModifiers(),
        GetVirtualCode()));
    isRegistered = true;
}

void HotkeyInfo::Unregister()
{
    if(!isRegistered)
    {
        return;
    }

    AssertWinApiResult(UnregisterHotKey(GetWindowHandle(), GetHotkeyId()));
    isRegistered = false;
}

bool HotkeyInfo::IsRegistered() const
{
    return isRegistered;
}

void HotkeyInfo::UpdateHotkey(int hotkey)
{
    modifiers = HIBYTE(LOWORD(hotkey));
    virtualCode = LOBYTE(LOWORD(hotkey));
}
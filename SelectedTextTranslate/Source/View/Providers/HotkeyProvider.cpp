#include "View\Providers\HotkeyProvider.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

HotkeyProvider::HotkeyProvider()
{
    this->hotkeyCallbacks = map<int, function<void()>>();
}

void HotkeyProvider::RegisterHotkeyCallback(int hotkeyId, function<void()> pressedCallback)
{
    hotkeyCallbacks[hotkeyId] = pressedCallback;
}

void HotkeyProvider::UnregisterHotkeyCallback(int hotkeyId)
{
    if(hotkeyCallbacks.count(hotkeyId) != 0)
    {
        hotkeyCallbacks.erase(hotkeyId);
    }
}

void HotkeyProvider::RegisterCustomHotkey(HWND windowHandle, function<void()> pressedCallback, int hotkeyId, UINT modifiers, UINT virtualCode)
{
    AssertWinApiResult(RegisterHotKey(windowHandle, hotkeyId, modifiers, virtualCode));
    RegisterHotkeyCallback(hotkeyId, pressedCallback);
}

void HotkeyProvider::UnregisterCustomHotkey(HWND windowHandle, int hotkeyId)
{
    AssertWinApiResult(UnregisterHotKey(windowHandle, hotkeyId));
    UnregisterHotkeyCallback(hotkeyId);
}

void HotkeyProvider::RegisterTranslateHotkey(HWND windowHandle, function<void()> pressedCallback)
{
    RegisterCustomHotkey(windowHandle, pressedCallback, TranslateHotkeyId, MOD_CONTROL, 0x54/*T*/);
}

void HotkeyProvider::RegisterPlayTextHotkey(HWND windowHandle, function<void()> pressedCallback)
{
    RegisterCustomHotkey(windowHandle, pressedCallback, PlayTextHotkeyId, MOD_CONTROL, 0x52/*R*/);
}

void HotkeyProvider::RegisterZoomInHotkey(HWND windowHandle, function<void()> pressedCallback)
{
    RegisterCustomHotkey(windowHandle, pressedCallback, ZoomInHotkeyPrimaryId, MOD_CONTROL, 0x6B/*Numpad plus*/);
    RegisterCustomHotkey(windowHandle, pressedCallback, ZoomInHotkeySecondaryId, MOD_CONTROL, VK_OEM_PLUS);
}

void HotkeyProvider::RegisterZoomOutHotkey(HWND windowHandle, function<void()> pressedCallback)
{
    RegisterCustomHotkey(windowHandle, pressedCallback, ZoomOutHotkeyPrimaryId, MOD_CONTROL, 0x6d/*Numpad minus*/);
    RegisterCustomHotkey(windowHandle, pressedCallback, ZoomOutHotkeySecondaryId, MOD_CONTROL, VK_OEM_MINUS);
}

void HotkeyProvider::UnregisterZoomInHotkey(HWND windowHandle)
{
    UnregisterCustomHotkey(windowHandle, ZoomInHotkeyPrimaryId);
    UnregisterCustomHotkey(windowHandle, ZoomInHotkeySecondaryId);
}

void HotkeyProvider::UnregisterZoomOutHotkey(HWND windowHandle)
{
    UnregisterCustomHotkey(windowHandle, ZoomOutHotkeyPrimaryId);
    UnregisterCustomHotkey(windowHandle, ZoomOutHotkeySecondaryId);
}

void HotkeyProvider::ProcessHotkey(DWORD hotkeyId)
{
    if(hotkeyCallbacks.count(hotkeyId) != 0)
    {
        hotkeyCallbacks[hotkeyId]();
    }
}

HotkeyProvider::~HotkeyProvider()
{
}

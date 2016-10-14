#include "Windows\Providers\HotkeyProvider.h"

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

void HotkeyProvider::RegisterTranslateHotkey(HWND windowHandle, function<void()> pressedCallback)
{
    RegisterHotKey(windowHandle, TranslateHotkeyId, MOD_CONTROL, 0x54/*T*/);
    RegisterHotkeyCallback(TranslateHotkeyId, pressedCallback);
}

void HotkeyProvider::RegisterPlayTextHotkey(HWND windowHandle, function<void()> pressedCallback)
{
    RegisterHotKey(windowHandle, PlayTextHotkeyId, MOD_CONTROL, 0x52/*R*/);
    RegisterHotkeyCallback(PlayTextHotkeyId, pressedCallback);
}

void HotkeyProvider::RegisterZoomInHotkey(HWND windowHandle, function<void()> pressedCallback)
{
    RegisterHotKey(windowHandle, ZoomInHotkeyPrimaryId, MOD_CONTROL, 0x6B/*Numpad plus*/);
    RegisterHotKey(windowHandle, ZoomInHotkeySecondaryId, MOD_CONTROL, VK_OEM_PLUS);
    RegisterHotkeyCallback(ZoomInHotkeyPrimaryId, pressedCallback);
    RegisterHotkeyCallback(ZoomInHotkeySecondaryId, pressedCallback);
}

void HotkeyProvider::RegisterZoomOutHotkey(HWND windowHandle, function<void()> pressedCallback)
{
    RegisterHotKey(windowHandle, ZoomOutHotkeyPrimaryId, MOD_CONTROL, 0x6d/*Numpad minus*/);
    RegisterHotKey(windowHandle, ZoomOutHotkeySecondaryId, MOD_CONTROL, VK_OEM_MINUS);
    RegisterHotkeyCallback(ZoomOutHotkeyPrimaryId, pressedCallback);
    RegisterHotkeyCallback(ZoomOutHotkeySecondaryId, pressedCallback);
}

void HotkeyProvider::UnregisterZoomInHotkey(HWND windowHandle)
{
    UnregisterHotKey(windowHandle, ZoomInHotkeyPrimaryId);
    UnregisterHotKey(windowHandle, ZoomInHotkeySecondaryId);
    UnregisterHotkeyCallback(ZoomInHotkeyPrimaryId);
    UnregisterHotkeyCallback(ZoomInHotkeySecondaryId);
}

void HotkeyProvider::UnregisterZoomOutHotkey(HWND windowHandle)
{
    UnregisterHotKey(windowHandle, ZoomOutHotkeyPrimaryId);
    UnregisterHotKey(windowHandle, ZoomOutHotkeySecondaryId);
    UnregisterHotkeyCallback(ZoomOutHotkeyPrimaryId);
    UnregisterHotkeyCallback(ZoomOutHotkeySecondaryId);
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

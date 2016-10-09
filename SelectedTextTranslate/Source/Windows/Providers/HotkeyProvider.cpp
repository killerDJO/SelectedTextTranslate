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
    RegisterHotKey(windowHandle, ZoomInHotkeyId, MOD_CONTROL, 0x6B/*Numpad plus*/);
    RegisterHotkeyCallback(ZoomInHotkeyId, pressedCallback);
}

void HotkeyProvider::RegisterZoomOutHotkey(HWND windowHandle, function<void()> pressedCallback)
{
    RegisterHotKey(windowHandle, ZoomOutHotkeyId, MOD_CONTROL, 0x6d/*Numpad minus*/);
    RegisterHotkeyCallback(ZoomOutHotkeyId, pressedCallback);
}

void HotkeyProvider::UnregisterZoomInHotkey(HWND windowHandle)
{
    UnregisterHotKey(windowHandle, ZoomInHotkeyId);
    UnregisterHotkeyCallback(ZoomInHotkeyId);
}

void HotkeyProvider::UnregisterZoomOutHotkey(HWND windowHandle)
{
    UnregisterHotKey(windowHandle, ZoomOutHotkeyId);
    UnregisterHotkeyCallback(ZoomOutHotkeyId);
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

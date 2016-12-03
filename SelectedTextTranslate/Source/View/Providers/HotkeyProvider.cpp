#include "View\Providers\HotkeyProvider.h"

HotkeyProvider::HotkeyProvider(HotkeySettings settings, MessageBus* messageBus)
{
    this->hotkeysRegistry = vector<HotkeyInfo>();
    this->isSuspended = false;
    this->hotkeyIdToHotkeyMap = map<int, int>();

    SetHotkeysSettings(settings);

    messageBus->OnSuspendHotkeys.Subscribe(bind(&HotkeyProvider::SuspendHotkeys, this));
    messageBus->OnEnableHotkeys.Subscribe(bind(&HotkeyProvider::EnableHotkeys, this));
}

void HotkeyProvider::SetHotkeysSettings(HotkeySettings settings)
{
    hotkeyIdToHotkeyMap[TranslateHotkeyId] = settings.GetTranslateHotkey();
    hotkeyIdToHotkeyMap[PlayTextHotkeyId] = settings.GetPlayTextHotkey();
    hotkeyIdToHotkeyMap[ZoomInHotkeyPrimaryId] = settings.GetZoomInHotkey();
    hotkeyIdToHotkeyMap[ZoomOutHotkeyPrimaryId] = settings.GetZoomOutHotkey();

    SuspendHotkeys();
    UpdateHotkeysInfo();
    EnableHotkeys();
}

void HotkeyProvider::RegisterCustomHotkey(HotkeyInfo hotkeyInfo)
{
    if(!isSuspended)
    {
        hotkeyInfo.Register();
    }
    
    hotkeysRegistry.push_back(hotkeyInfo);
}

void HotkeyProvider::UnregisterCustomHotkey(HWND windowHandle, int hotkeyId)
{
    for(size_t i = 0; i < hotkeysRegistry.size(); ++i)
    {
        HotkeyInfo hotkeyInfo = hotkeysRegistry[i];
        if(hotkeyInfo.GetHotkeyId() == hotkeyId && hotkeyInfo.GetWindowHandle() == windowHandle)
        {
            hotkeyInfo.Unregister();
            hotkeysRegistry.erase(hotkeysRegistry.begin() + i);
            break;
        }
    }
}

void HotkeyProvider::RegisterTranslateHotkey(HWND windowHandle, function<void()> pressedCallback)
{
    RegisterCustomHotkey(HotkeyInfo(windowHandle, pressedCallback, TranslateHotkeyId, hotkeyIdToHotkeyMap[TranslateHotkeyId]));
}

void HotkeyProvider::RegisterPlayTextHotkey(HWND windowHandle, function<void()> pressedCallback)
{
    RegisterCustomHotkey(HotkeyInfo(windowHandle, pressedCallback, PlayTextHotkeyId, hotkeyIdToHotkeyMap[PlayTextHotkeyId]));
}

void HotkeyProvider::RegisterZoomInHotkey(HWND windowHandle, function<void()> pressedCallback)
{
    RegisterCustomHotkey(HotkeyInfo(windowHandle, pressedCallback, ZoomInHotkeyPrimaryId, hotkeyIdToHotkeyMap[ZoomInHotkeyPrimaryId]));
}

void HotkeyProvider::RegisterZoomOutHotkey(HWND windowHandle, function<void()> pressedCallback)
{
    RegisterCustomHotkey(HotkeyInfo(windowHandle, pressedCallback, ZoomOutHotkeyPrimaryId, hotkeyIdToHotkeyMap[ZoomOutHotkeyPrimaryId]));
}

void HotkeyProvider::UnregisterZoomInHotkey(HWND windowHandle)
{
    UnregisterCustomHotkey(windowHandle, ZoomInHotkeyPrimaryId);
}

void HotkeyProvider::UnregisterZoomOutHotkey(HWND windowHandle)
{
    UnregisterCustomHotkey(windowHandle, ZoomOutHotkeyPrimaryId);
}

void HotkeyProvider::SuspendHotkeys()
{
    isSuspended = true;
    for (size_t i = 0; i < hotkeysRegistry.size(); ++i)
    {
        hotkeysRegistry[i].Unregister();
    }
}

void HotkeyProvider::EnableHotkeys()
{
    isSuspended = false;
    for (size_t i = 0; i < hotkeysRegistry.size(); ++i)
    {
        hotkeysRegistry[i].Register();
    }
}

void HotkeyProvider::UpdateHotkeysInfo()
{
    for (size_t i = 0; i < hotkeysRegistry.size(); ++i)
    {
        hotkeysRegistry[i].UpdateHotkey(hotkeyIdToHotkeyMap[hotkeysRegistry[i].GetHotkeyId()]);
    }
}

void HotkeyProvider::ProcessHotkey(DWORD hotkeyId)
{
    for (size_t i = 0; i < hotkeysRegistry.size(); ++i)
    {
        if (hotkeysRegistry[i].GetHotkeyId() == hotkeyId)
        {
            hotkeysRegistry[i].GetPressedCallback()();
        }
    }
}

HotkeyProvider::~HotkeyProvider()
{
}

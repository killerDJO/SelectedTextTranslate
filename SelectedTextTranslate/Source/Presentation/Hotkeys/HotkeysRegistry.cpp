#include "Presentation\Hotkeys\HotkeysRegistry.h"
#include "Presentation\MessageBus.h"
#include "BusinessLogic\Settings\SettingsProvider.h"

HotkeysRegistry::HotkeysRegistry(ServiceRegistry* registry)
{
    this->hotkeysRegistry = vector<HotkeyInfo>();
    this->isSuspended = false;
    this->hotkeyIdToHotkeyMap = map<int, int>();

    SetHotkeysSettings(registry->Get<SettingsProvider>()->GetSettings().GetHotkeySettings());

    registry->Get<MessageBus>()->OnSuspendHotkeys.Subscribe(bind(&HotkeysRegistry::SuspendHotkeys, this));
    registry->Get<MessageBus>()->OnEnableHotkeys.Subscribe(bind(&HotkeysRegistry::EnableHotkeys, this));
}

void HotkeysRegistry::SetHotkeysSettings(HotkeySettings settings)
{
    hotkeyIdToHotkeyMap[TranslateHotkeyId] = settings.GetTranslateHotkey();
    hotkeyIdToHotkeyMap[PlayTextHotkeyId] = settings.GetPlayTextHotkey();
    hotkeyIdToHotkeyMap[ZoomInHotkeyId] = settings.GetZoomInHotkey();
    hotkeyIdToHotkeyMap[ZoomOutHotkeyId] = settings.GetZoomOutHotkey();

    SuspendHotkeys();
    UpdateHotkeysInfo();
    EnableHotkeys();
}

void HotkeysRegistry::RegisterCustomHotkey(HotkeyInfo hotkeyInfo)
{
    if(!isSuspended)
    {
        hotkeyInfo.Register();
    }
    
    hotkeysRegistry.push_back(hotkeyInfo);
}

void HotkeysRegistry::UnregisterCustomHotkey(HWND windowHandle, int hotkeyId)
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

void HotkeysRegistry::RegisterTranslateHotkey(HWND windowHandle, function<void()> pressedCallback)
{
    RegisterCustomHotkey(HotkeyInfo(windowHandle, pressedCallback, TranslateHotkeyId, hotkeyIdToHotkeyMap[TranslateHotkeyId]));
}

void HotkeysRegistry::RegisterPlayTextHotkey(HWND windowHandle, function<void()> pressedCallback)
{
    RegisterCustomHotkey(HotkeyInfo(windowHandle, pressedCallback, PlayTextHotkeyId, hotkeyIdToHotkeyMap[PlayTextHotkeyId]));
}

void HotkeysRegistry::RegisterZoomInHotkey(HWND windowHandle, function<void()> pressedCallback)
{
    RegisterCustomHotkey(HotkeyInfo(windowHandle, pressedCallback, ZoomInHotkeyId, hotkeyIdToHotkeyMap[ZoomInHotkeyId]));
}

void HotkeysRegistry::RegisterZoomOutHotkey(HWND windowHandle, function<void()> pressedCallback)
{
    RegisterCustomHotkey(HotkeyInfo(windowHandle, pressedCallback, ZoomOutHotkeyId, hotkeyIdToHotkeyMap[ZoomOutHotkeyId]));
}

void HotkeysRegistry::UnregisterZoomInHotkey(HWND windowHandle)
{
    UnregisterCustomHotkey(windowHandle, ZoomInHotkeyId);
}

void HotkeysRegistry::UnregisterZoomOutHotkey(HWND windowHandle)
{
    UnregisterCustomHotkey(windowHandle, ZoomOutHotkeyId);
}

void HotkeysRegistry::UnregisterTranslateHotkey(HWND windowHandle)
{
    UnregisterCustomHotkey(windowHandle, TranslateHotkeyId);
}

void HotkeysRegistry::UnregisterPlayTextHotkey(HWND windowHandle)
{
    UnregisterCustomHotkey(windowHandle, PlayTextHotkeyId);
}

void HotkeysRegistry::SuspendHotkeys()
{
    isSuspended = true;
    for (size_t i = 0; i < hotkeysRegistry.size(); ++i)
    {
        hotkeysRegistry[i].Unregister();
    }
}

void HotkeysRegistry::EnableHotkeys()
{
    isSuspended = false;
    for (size_t i = 0; i < hotkeysRegistry.size(); ++i)
    {
        hotkeysRegistry[i].Register();
    }
}

void HotkeysRegistry::UpdateHotkeysInfo()
{
    for (size_t i = 0; i < hotkeysRegistry.size(); ++i)
    {
        hotkeysRegistry[i].UpdateHotkey(hotkeyIdToHotkeyMap[hotkeysRegistry[i].GetHotkeyId()]);
    }
}

void HotkeysRegistry::ProcessHotkey(DWORD hotkeyId)
{
    for (size_t i = 0; i < hotkeysRegistry.size(); ++i)
    {
        if (hotkeysRegistry[i].GetHotkeyId() == hotkeyId)
        {
            hotkeysRegistry[i].GetPressedCallback()();
        }
    }
}
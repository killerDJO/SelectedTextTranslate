#pragma once
#include "BusinessLogic\Settings\Dto\HotkeySettings.h"
#include "Presentation\Providers\Dto\HotkeyInfo.h"
#include "Infrastructure\ServiceRegistry\ServiceRegistry.h"

class HotkeysRegistry
{
private:
    const int TranslateHotkeyId = 3003;
    const int PlayTextHotkeyId = 3004;
    const int ZoomInHotkeyId = 3006;
    const int ZoomOutHotkeyId = 3007;

    map<int, int> hotkeyIdToHotkeyMap;
    vector<HotkeyInfo> hotkeysRegistry;
    bool isSuspended;

    void RegisterCustomHotkey(HotkeyInfo hotkeyInfo);
    void UnregisterCustomHotkey(HWND windowHandle, int hotkeyId);
    void UpdateHotkeysInfo();

public:
    HotkeysRegistry(ServiceRegistry* registry);

    void SetHotkeysSettings(HotkeySettings settings);

    void RegisterTranslateHotkey(HWND windowHandle, function<void()> pressedCallback);
    void RegisterPlayTextHotkey(HWND windowHandle, function<void()> pressedCallback);

    void RegisterZoomInHotkey(HWND windowHandle, function<void()> pressedCallback);
    void RegisterZoomOutHotkey(HWND windowHandle, function<void()> pressedCallback);

    void UnregisterZoomInHotkey(HWND windowHandle);
    void UnregisterZoomOutHotkey(HWND windowHandle);
    void UnregisterTranslateHotkey(HWND windowHandle);
    void UnregisterPlayTextHotkey(HWND windowHandle);

    void SuspendHotkeys();
    void EnableHotkeys();

    void ProcessHotkey(DWORD hotkeyId);
};

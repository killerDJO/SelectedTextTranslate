#pragma once

class HotkeyProvider
{
private:
    const int TranslateHotkeyId = 3003;
    const int PlayTextHotkeyId = 3004;
    const int ZoomInHotkeyPrimaryId = 3006;
    const int ZoomOutHotkeyPrimaryId = 3007;
    const int ZoomInHotkeySecondaryId = 3008;
    const int ZoomOutHotkeySecondaryId = 3009;

    map<int, function<void()>> hotkeyCallbacks;

    void RegisterHotkeyCallback(int hotkeyId, function<void()> pressedCallback);
    void UnregisterHotkeyCallback(int hotkeyId);

    void RegisterCustomHotkey(HWND windowHandle, function<void()> pressedCallback, int hotkeyId, UINT modifiers, UINT virtualCode);
    void UnregisterCustomHotkey(HWND windowHandle, int hotkeyId);

public:
    HotkeyProvider();
    ~HotkeyProvider();

    void RegisterTranslateHotkey(HWND windowHandle, function<void()> pressedCallback);
    void RegisterPlayTextHotkey(HWND windowHandle, function<void()> pressedCallback);

    void RegisterZoomInHotkey(HWND windowHandle, function<void()> pressedCallback);
    void RegisterZoomOutHotkey(HWND windowHandle, function<void()> pressedCallback);

    void UnregisterZoomInHotkey(HWND windowHandle);
    void UnregisterZoomOutHotkey(HWND windowHandle);

    void ProcessHotkey(DWORD hotkeyId);
};
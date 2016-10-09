#pragma once

class HotkeyProvider
{
private:
    const int TranslateHotkeyId = 3003;
    const int PlayTextHotkeyId = 3004;
    const int ZoomInHotkeyId = 3006;
    const int ZoomOutHotkeyId = 3007;

    map<int, function<void()>> hotkeyCallbacks;

    void RegisterHotkeyCallback(int hotkeyId, function<void()> pressedCallback);
    void UnregisterHotkeyCallback(int hotkeyId);

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
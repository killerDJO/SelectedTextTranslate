#pragma once
class HotkeyInfo
{
private:
    HWND windowHandle;
    function<void()> pressedCallback;
    int hotkeyId;
    UINT modifiers;
    UINT virtualCode;

    bool isRegistered;

public:
    HotkeyInfo();
    HotkeyInfo(HWND windowHandle, function<void()> pressedCallback, int hotkeyId, UINT modifiers, UINT virtualCode);
    HotkeyInfo(HWND windowHandle, function<void()> pressedCallback, int hotkeyId, int hotkey);

    HWND GetWindowHandle() const;
    function<void()> GetPressedCallback() const;
    int GetHotkeyId() const;
    UINT GetModifiers() const;
    UINT GetVirtualCode() const;

    void Register();
    void Unregister();
    bool IsRegistered() const;
    void UpdateHotkey(int hotkey);
};
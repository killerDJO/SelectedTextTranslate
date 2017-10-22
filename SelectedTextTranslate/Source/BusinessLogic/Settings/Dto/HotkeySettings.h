#pragma once

class HotkeySettings
{
private:
    DWORD translateHotkey;
    DWORD playTextHotkey;
    DWORD zoomInHotkey;
    DWORD zoomOutHotkey;

public:
    HotkeySettings(DWORD translateHotkey, DWORD playTextHotkey, DWORD zoomInHotkey, DWORD zoomOutHotkey);
    HotkeySettings();

    DWORD GetTranslateHotkey() const;
    void SetTranslateHotkey(DWORD translateHotkey);

    DWORD GetPlayTextHotkey() const;
    void SetPlayTextHotkey(DWORD playTextHotkey);

    DWORD GetZoomInHotkey() const;
    void SetZoomInHotkey(DWORD zoomInHotkey);

    DWORD GetZoomOutHotkey() const;
    void SetZoomOutHotkey(DWORD zoomOutHotkey);

    bool EqualTo(HotkeySettings settings) const;
};
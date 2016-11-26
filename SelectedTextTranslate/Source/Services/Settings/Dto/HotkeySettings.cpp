#include "Services\Settings\Dto\HotkeySettings.h"

HotkeySettings::HotkeySettings(DWORD translateHotkey, DWORD playTextHotkey, DWORD zoomInHotkey, DWORD zoomOutHotkey)
{
    this->translateHotkey = translateHotkey;
    this->playTextHotkey = playTextHotkey;
    this->zoomInHotkey = zoomInHotkey;
    this->zoomOutHotkey = zoomOutHotkey;
}

HotkeySettings::HotkeySettings()
{
    this->translateHotkey = 0;
    this->playTextHotkey = 0;
    this->zoomInHotkey = 0;
    this->zoomOutHotkey = 0;
}

DWORD HotkeySettings::GetTranslateHotkey() const
{
    return translateHotkey;
}

void HotkeySettings::SetTranslateHotkey(DWORD translateHotkey)
{
    this->translateHotkey = translateHotkey;
}

DWORD HotkeySettings::GetPlayTextHotkey() const
{
    return playTextHotkey;
}

void HotkeySettings::SetPlayTextHotkey(DWORD playTextHotkey)
{
    this->playTextHotkey = playTextHotkey;
}

DWORD HotkeySettings::GetZoomInHotkey() const
{
    return zoomInHotkey;
}

void HotkeySettings::SetZoomInHotkey(DWORD zoomInHotkey)
{
    this->zoomInHotkey = zoomInHotkey;
}

DWORD HotkeySettings::GetZoomOutHotkey() const
{
    return zoomOutHotkey;
}

void HotkeySettings::SetZoomOutHotkey(DWORD zoomOutHotkey)
{
    this->zoomOutHotkey = zoomOutHotkey;
}

bool HotkeySettings::EqualTo(HotkeySettings settings) const
{
    return settings.GetPlayTextHotkey() == GetPlayTextHotkey()
        && settings.GetTranslateHotkey() == GetTranslateHotkey()
        && settings.GetZoomInHotkey() == GetZoomInHotkey()
        && settings.GetZoomOutHotkey() == GetZoomOutHotkey();
}

HotkeySettings::~HotkeySettings()
{
}

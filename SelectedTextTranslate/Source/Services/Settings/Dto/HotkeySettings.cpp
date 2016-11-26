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
    this->translateHotkey = 596 /* CTRL + T */;
    this->playTextHotkey = 594 /* CTRL + R */;
    this->zoomInHotkey = 619 /* CTRL + NUM_PLUS */;
    this->zoomOutHotkey = 621 /* CTRL + NUM_MINUS */;
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

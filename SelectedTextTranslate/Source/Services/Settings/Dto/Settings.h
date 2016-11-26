#pragma once
#include "Services\Settings\Dto\HotkeySettings.h"

using namespace web;

class Settings
{
private:
    HotkeySettings hotkeySettings;

    static HotkeySettings ParseHotkeySettings(json::value hotkeySettingsJson);
    static json::value SerializeHotkeySettings(HotkeySettings hotkeySettings);

public:
    Settings(HotkeySettings hotkeySettings);
    Settings();
    ~Settings();

    HotkeySettings GetHotkeySettings() const;
    void SetHotkeySettings(HotkeySettings hotkeySettings);

    static wstring SerializeToJson(Settings settings);
    static Settings ParseFromJson(wstring json);

    bool EqualTo(Settings settings) const;
};
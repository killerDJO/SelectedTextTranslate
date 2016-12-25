#pragma once
#include "Services\Settings\Dto\HotkeySettings.h"

using namespace nlohmann;

class Settings
{
private:
    static const char* HotkeySettingsKey;
    static const char* TranslateHotkeyKey;
    static const char* PlayTextHotkeyKey;
    static const char* ZoomInHotkeyKey;
    static const char* ZoomOutHotkeyKey;

    HotkeySettings hotkeySettings;

    static HotkeySettings ParseHotkeySettings(json hotkeySettingsJson);
    static json SerializeHotkeySettings(HotkeySettings hotkeySettings);

public:
    Settings(HotkeySettings hotkeySettings);
    Settings();

    HotkeySettings GetHotkeySettings() const;
    void SetHotkeySettings(HotkeySettings hotkeySettings);

    static wstring SerializeToJson(Settings settings);
    static Settings ParseFromJson(wstring jsonContent);

    bool EqualTo(Settings settings) const;
};
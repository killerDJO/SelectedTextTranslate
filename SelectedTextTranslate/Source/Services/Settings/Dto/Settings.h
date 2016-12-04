#pragma once
#include "Services\Settings\Dto\HotkeySettings.h"
#include "json.hpp"

using namespace nlohmann;

class Settings
{
private:
    HotkeySettings hotkeySettings;

    static HotkeySettings ParseHotkeySettings(json hotkeySettingsJson);
    static json SerializeHotkeySettings(HotkeySettings hotkeySettings);

public:
    Settings(HotkeySettings hotkeySettings);
    Settings();
    ~Settings();

    HotkeySettings GetHotkeySettings() const;
    void SetHotkeySettings(HotkeySettings hotkeySettings);

    static wstring SerializeToJson(Settings settings);
    static Settings ParseFromJson(wstring jsonContent);

    bool EqualTo(Settings settings) const;
};
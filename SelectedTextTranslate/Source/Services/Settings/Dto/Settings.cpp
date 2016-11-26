#include "Services\Settings\Dto\Settings.h"

Settings::Settings(HotkeySettings hotkeySettings)
{
    this->hotkeySettings = hotkeySettings;
}

Settings::Settings()
{
    this->hotkeySettings = HotkeySettings();
}

HotkeySettings Settings::GetHotkeySettings() const
{
    return hotkeySettings;
}

void Settings::SetHotkeySettings(HotkeySettings hotkeySettings)
{
    this->hotkeySettings = hotkeySettings;
}

wstring Settings::SerializeToJson(Settings settings)
{
    json::value settingsJson;

    settingsJson[L"hotkeySettings"] = SerializeHotkeySettings(settings.GetHotkeySettings());

    return settingsJson.serialize();
}

json::value Settings::SerializeHotkeySettings(HotkeySettings hotkeySettings)
{
    json::value hotkeySettingsJson;
    hotkeySettingsJson[L"translateHotkey"] = json::value::number((uint32_t)hotkeySettings.GetTranslateHotkey());
    hotkeySettingsJson[L"playTextHotkey"] = json::value::number((uint32_t)hotkeySettings.GetPlayTextHotkey());
    hotkeySettingsJson[L"zoomInHotkey"] = json::value::number((uint32_t)hotkeySettings.GetZoomInHotkey());
    hotkeySettingsJson[L"zoomOutHotkey"] = json::value::number((uint32_t)hotkeySettings.GetZoomOutHotkey());
    return hotkeySettingsJson;
}

Settings Settings::ParseFromJson(wstring json)
{
    json::value settingsJson = json::value::parse(json);

    if (settingsJson.is_null())
    {
        return Settings();
    }

    HotkeySettings hotkeySettings = ParseHotkeySettings(settingsJson[L"hotkeySettings"]);

    return Settings(hotkeySettings);
}

HotkeySettings Settings::ParseHotkeySettings(json::value hotkeySettingsJson)
{
    DWORD translateHotkey = hotkeySettingsJson[L"translateHotkey"].as_integer();
    DWORD playTextHotkey = hotkeySettingsJson[L"playTextHotkey"].as_integer();
    DWORD zoomInHotkey = hotkeySettingsJson[L"zoomInHotkey"].as_integer();
    DWORD zoomOutHotkey = hotkeySettingsJson[L"zoomOutHotkey"].as_integer();

    return HotkeySettings(translateHotkey, playTextHotkey, zoomInHotkey, zoomOutHotkey);
}

bool Settings::EqualTo(Settings settings) const
{
    return settings.GetHotkeySettings().EqualTo(GetHotkeySettings());
}

Settings::~Settings()
{
}

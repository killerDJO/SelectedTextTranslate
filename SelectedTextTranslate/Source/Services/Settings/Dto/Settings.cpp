#include "Services\Settings\Dto\Settings.h"
#include "Utilities\StringUtilities.h"

const char* Settings::HotkeySettingsKey = "hotkeySettings";
const char* Settings::TranslateHotkeyKey = "translateHotkey";
const char* Settings::PlayTextHotkeyKey = "playTextHotkey";
const char* Settings::ZoomInHotkeyKey = "zoomInHotkey";
const char* Settings::ZoomOutHotkeyKey = "zoomOutHotkey";

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
    json settingsJson =
    {
        { HotkeySettingsKey, SerializeHotkeySettings(settings.GetHotkeySettings()) }
    };

    return StringUtilities::GetUtf16String(settingsJson.dump(4));
}

json Settings::SerializeHotkeySettings(HotkeySettings hotkeySettings)
{
    return
    {
        { TranslateHotkeyKey, hotkeySettings.GetTranslateHotkey() },
        { PlayTextHotkeyKey, hotkeySettings.GetPlayTextHotkey() },
        { ZoomInHotkeyKey, hotkeySettings.GetZoomInHotkey() },
        { ZoomOutHotkeyKey, hotkeySettings.GetZoomOutHotkey() }
    };
}

Settings Settings::ParseFromJson(wstring jsonContent)
{
    json settingsJson = json::parse(StringUtilities::GetUtf8String(jsonContent).c_str());
    if (settingsJson.empty())
    {
        return Settings();
    }

    HotkeySettings hotkeySettings = ParseHotkeySettings(settingsJson[HotkeySettingsKey]);

    return Settings(hotkeySettings);
}

HotkeySettings Settings::ParseHotkeySettings(json hotkeySettingsJson)
{
    DWORD translateHotkey = hotkeySettingsJson[TranslateHotkeyKey].get<DWORD>();
    DWORD playTextHotkey = hotkeySettingsJson[PlayTextHotkeyKey].get<DWORD>();
    DWORD zoomInHotkey = hotkeySettingsJson[ZoomInHotkeyKey].get<DWORD>();
    DWORD zoomOutHotkey = hotkeySettingsJson[ZoomOutHotkeyKey].get<DWORD>();

    return HotkeySettings(translateHotkey, playTextHotkey, zoomInHotkey, zoomOutHotkey);
}

bool Settings::EqualTo(Settings settings) const
{
    return settings.GetHotkeySettings().EqualTo(GetHotkeySettings());
}
#include "Services\Settings\Dto\Settings.h"
#include "Utilities\StringUtilities.h"

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
        { "hotkeySettings", SerializeHotkeySettings(settings.GetHotkeySettings()) }
    };

    return StringUtilities::GetUtf16String(settingsJson.dump(4));
}

json Settings::SerializeHotkeySettings(HotkeySettings hotkeySettings)
{
    return
    {
        { "translateHotkey", hotkeySettings.GetTranslateHotkey() },
        { "playTextHotkey", hotkeySettings.GetPlayTextHotkey() },
        { "zoomInHotkey", hotkeySettings.GetZoomInHotkey() },
        { "zoomOutHotkey", hotkeySettings.GetZoomOutHotkey() }
    };
}

Settings Settings::ParseFromJson(wstring jsonContent)
{
    json settingsJson = json::parse(StringUtilities::GetUtf8String(jsonContent).c_str());
    if (settingsJson.empty())
    {
        return Settings();
    }

    HotkeySettings hotkeySettings = ParseHotkeySettings(settingsJson["hotkeySettings"]);

    return Settings(hotkeySettings);
}

HotkeySettings Settings::ParseHotkeySettings(json hotkeySettingsJson)
{
    DWORD translateHotkey = hotkeySettingsJson["translateHotkey"].get<DWORD>();
    DWORD playTextHotkey = hotkeySettingsJson["playTextHotkey"].get<DWORD>();
    DWORD zoomInHotkey = hotkeySettingsJson["zoomInHotkey"].get<DWORD>();
    DWORD zoomOutHotkey = hotkeySettingsJson["zoomOutHotkey"].get<DWORD>();

    return HotkeySettings(translateHotkey, playTextHotkey, zoomInHotkey, zoomOutHotkey);
}

bool Settings::EqualTo(Settings settings) const
{
    return settings.GetHotkeySettings().EqualTo(GetHotkeySettings());
}

Settings::~Settings()
{
}

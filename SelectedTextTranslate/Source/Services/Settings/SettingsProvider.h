#pragma once
#include "Infrastructure\Logging\Logger.h"
#include "Services\Settings\Dto\Settings.h"

class SettingsProvider
{
private:
    Logger* logger;
    wstring settingsFileName;

    Settings CreateDefaultSettings() const;

public:
    SettingsProvider(Logger* logger);

    Settings GetSettings() const;
    void UpdateSettings(Settings settings) const;
};

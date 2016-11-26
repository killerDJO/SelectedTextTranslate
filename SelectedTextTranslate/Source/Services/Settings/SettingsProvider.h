#pragma once
#include "Services\Settings\Dto\Settings.h"
#include "Infrastructure\Logging\Logger.h"

class SettingsProvider
{
private:
    Logger* logger;
    wstring settingsFileName;

    Settings CreateDefaultSettings() const;

public:
    SettingsProvider(Logger* logger);
    ~SettingsProvider();

    Settings GetSettings() const;
    void UpdateSettings(Settings settings) const;
};

#pragma once
#include "Infrastructure\Logging\Logger.h"
#include "BusinessLogic\Settings\Dto\Settings.h"
#include "Infrastructure\ServiceRegistry\ServiceRegistry.h"

class SettingsProvider
{
private:
    Logger* logger;
    wstring settingsFileName;

    Settings CreateDefaultSettings() const;

public:
    SettingsProvider(ServiceRegistry* registry);

    Settings GetSettings() const;
    void UpdateSettings(Settings settings) const;
};

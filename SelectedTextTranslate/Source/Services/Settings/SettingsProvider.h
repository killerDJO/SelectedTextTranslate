#pragma once
#include "Infrastructure\Logging\Logger.h"
#include "Services\Settings\Dto\Settings.h"
#include "CompositionRoot.h"

class SettingsProvider
{
private:
    Logger* logger;
    wstring settingsFileName;

    Settings CreateDefaultSettings() const;

public:
    SettingsProvider(CompositionRoot* root);

    Settings GetSettings() const;
    void UpdateSettings(Settings settings) const;
};

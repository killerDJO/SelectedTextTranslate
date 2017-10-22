#pragma once
#include "Infrastructure\Logging\Logger.h"
#include "BusinessLogic\Settings\Dto\Settings.h"
#include "Infrastructure\CompositionRoot.h"

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

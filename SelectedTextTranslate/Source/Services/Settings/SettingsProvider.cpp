#include "Services\Settings\SettingsProvider.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"

SettingsProvider::SettingsProvider(CompositionRoot* root)
{
    this->logger = root->GetService<Logger>();
    this->settingsFileName = L"settings.json";
}

Settings SettingsProvider::GetSettings() const
{
    HANDLE hFile = CreateFile(settingsFileName.c_str(), GENERIC_READ, 0, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        return CreateDefaultSettings();
    }

    const int bufferSize = 65536;
    static char buffer[bufferSize];
    wstring json = L"";

    DWORD nRead;
    do
    {
        AssertCriticalWinApiResult(ReadFile(hFile, buffer, bufferSize, &nRead, nullptr));
        json += wstring((wchar_t*)buffer, nRead / sizeof(wchar_t));
    } while (nRead == bufferSize);

    AssertCriticalWinApiResult(CloseHandle(hFile));

    if (json.empty())
    {
        return CreateDefaultSettings();
    }

    try
    {
        return Settings::ParseFromJson(json);
    }
    catch (exception exception)
    {
        logger->LogFormatted(LogLevels::Error, L"Error loading settings. Exception: %hs. ", exception.what());
        return CreateDefaultSettings();
    }
}

void SettingsProvider::UpdateSettings(Settings settings) const
{
    wstring json = Settings::SerializeToJson(settings);

    HANDLE hFile = CreateFile(settingsFileName.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

    AssertCriticalWinApiHandleResult(hFile);

    DWORD nWritten;
    const wchar_t* buffer = json.c_str();
    AssertCriticalWinApiResult(WriteFile(hFile, buffer, wcslen(buffer) * sizeof(wchar_t), &nWritten, nullptr));

    AssertCriticalWinApiResult(CloseHandle(hFile));
}

Settings SettingsProvider::CreateDefaultSettings() const
{
    Settings defaultSettings = Settings();
    UpdateSettings(defaultSettings);
    return defaultSettings;
}
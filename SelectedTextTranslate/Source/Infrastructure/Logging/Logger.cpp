#include "Infrastructure\Logging\Logger.h"
#include "Utilities\StringUtilities.h"
#include <chrono>
#include <iomanip>

Logger::Logger()
{
    CreateDirectory(L".\\logs", nullptr);
}

void Logger::Log(wstring record) const
{
    HANDLE hFile = CreateFile(GetLogFileName().c_str(), FILE_APPEND_DATA, 0, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        return;
    }

    record = GetCurrentDateTime() + L" " + record + L"\r\n";

    DWORD nWritten;
    const wchar_t* buffer = record.c_str();
    WriteFile(hFile, buffer, wcslen(buffer) * sizeof(wchar_t), &nWritten, nullptr);

    CloseHandle(hFile);
}

void Logger::LogFormatted(wstring format, ...) const
{
    va_list args;
    va_start(args, format);
    Log(StringUtilities::Format(format, args));
    va_end(args);
}

wstring Logger::GetLogFileName() const
{
    wstring currentDate = GetCurrentDate();
    wstring computerName = GetLocalComputerName();

    wstring fullFileName = L"logs\\" + wstring(currentDate.begin(), currentDate.end()) + L"_" + computerName + L"_trace.txt";

    return fullFileName;
}

wstring Logger::GetLocalComputerName() const
{
    wchar_t buffer[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD bufCharCount = MAX_COMPUTERNAME_LENGTH + 1;

    GetComputerName(buffer, &bufCharCount);

    return wstring(buffer);
}

tm GetTimeInfo()
{
    auto now = chrono::system_clock::now();
    time_t time = chrono::system_clock::to_time_t(now);

    tm timeinfo;
    localtime_s(&timeinfo, &time);

    return timeinfo;
}

wstring Logger::GetCurrentDate() const
{
    tm timeinfo = GetTimeInfo();

    wchar_t buffer[30];
    wcsftime(buffer, 30, L"%d-%m-%Y", &timeinfo);

    wstring date = wstring(buffer);

    return date;
}

wstring Logger::GetCurrentDateTime() const
{
    tm timeinfo = GetTimeInfo();

    wchar_t buffer[30];
    wcsftime(buffer, 30, L"%d-%m-%Y %H:%M:%S", &timeinfo);

    auto now = chrono::system_clock::now();
    auto ms =
        chrono::duration_cast<chrono::milliseconds>(now.time_since_epoch()) -
        chrono::duration_cast<chrono::seconds>(now.time_since_epoch());

    wstringstream ss;
    ss << setw(3) << setfill(L'0') << ms.count();

    wstring dateTime = wstring(buffer) + L"." + ss.str();

    return dateTime;
}

Logger::~Logger()
{
}
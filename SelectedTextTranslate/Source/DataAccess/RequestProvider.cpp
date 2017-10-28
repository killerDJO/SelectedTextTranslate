#include "DataAccess\RequestProvider.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateException.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Utilities\StringUtilities.h"

RequestProvider::RequestProvider(ServiceRegistry* registry)
{
    logger = registry->Get<Logger>();
    buffer = new unsigned char[BufferSize];
}

wstring RequestProvider::GetStringResponse(const wstring host, const wstring url) const
{
    vector<unsigned char> response = GetResponse(host, url);
    
    wstring result = StringUtilities::GetUtf16String(string(response.begin(), response.end()));

    response.clear();

    return result;
}

vector<unsigned char> RequestProvider::GetResponse(const wstring host, const wstring url) const
{
    // TODO: move to config
    const wchar_t* userAgent = L"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/61.0.3163.100 Safari/537.36";
    
    HINTERNET internet = InternetOpenW(userAgent, INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0);
    AssertWinApiResult(internet);

    HINTERNET connect = InternetConnectW(internet, host.c_str(), INTERNET_DEFAULT_HTTPS_PORT, nullptr, nullptr, INTERNET_SERVICE_HTTP, 0, 0);
    AssertWinApiResult(connect);

    auto flags = INTERNET_FLAG_HYPERLINK |
        INTERNET_FLAG_SECURE |
        INTERNET_FLAG_IGNORE_CERT_CN_INVALID |
        INTERNET_FLAG_IGNORE_CERT_DATE_INVALID |
        INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
        INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS |
        INTERNET_FLAG_NO_AUTH |
        INTERNET_FLAG_NO_CACHE_WRITE |
        INTERNET_FLAG_NO_UI |
        INTERNET_FLAG_PRAGMA_NOCACHE |
        INTERNET_FLAG_RELOAD;
    HINTERNET request = HttpOpenRequestW(connect, L"GET", url.c_str(), L"HTTP/1.1", nullptr, nullptr, flags, NULL);
    AssertWinApiResult(request);

    AssertWinApiResult(HttpSendRequestW(request, nullptr, 0, nullptr, 0));

    AssertStatusCode(request);

    auto data = GetData(request);

    InternetCloseHandle(request);
    InternetCloseHandle(connect);
    InternetCloseHandle(internet);

    return data;
}

void RequestProvider::AssertStatusCode(HINTERNET request) const
{
    const int StatusBufferSize = 128;
    DWORD statusBufferLength = StatusBufferSize;
    wchar_t statusBuffer[StatusBufferSize];

    AssertWinApiResult(HttpQueryInfoW(request, HTTP_QUERY_STATUS_CODE, &statusBuffer, &statusBufferLength, NULL));
    auto statusCode = _wtoi(statusBuffer);

    const int OkStatusCode = 200;
    if (statusCode != OkStatusCode)
    {
        throw SelectedTextTranslateException(StringUtilities::Format(L"Invalid status code: %d.", statusCode));
    }
}

vector<unsigned char> RequestProvider::GetData(HINTERNET request) const
{
    DWORD numberOfBytesRead;
    vector<unsigned char> result;
    do
    {
        AssertWinApiResult(InternetReadFile(request, buffer, BufferSize - 1, &numberOfBytesRead));
        result.insert(result.end(), buffer, buffer + numberOfBytesRead);
    } while (numberOfBytesRead != 0);

    return result;
}

wstring RequestProvider::EscapeText(wstring text) const
{
    wstringstream result;
    result << setbase(16);
    for (wstring::const_iterator i = text.begin(); i != text.end(); ++i)
    {
        if (isalnum(*i))
            result << *i;
        else
            result << L'%' << setw(2) << setfill(L'0') << static_cast<unsigned int>(static_cast<unsigned char>(*i));
    }
    return result.str();
}
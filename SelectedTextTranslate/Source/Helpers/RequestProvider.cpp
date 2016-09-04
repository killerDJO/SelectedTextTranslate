#include "Helpers\RequestProvider.h"

using namespace web;
using namespace web::http;
using namespace web::http::client;

RequestProvider::RequestProvider(Logger* logger)
{
    this->logger = logger;
}

wstring RequestProvider::GetStringResponse(wstring url)
{
    vector<unsigned char> response = GetResponse(url);
    
    wstring result = StringUtilities::GetUtf16String(string(response.begin(), response.end()));

    response.clear();

    return result;
}

vector<unsigned char> RequestProvider::GetResponse(wstring url)
{
    try
    {
        uri newUri(url);
        http_client client(newUri);

        http_request request(methods::GET);
        request.headers().add(L"User-Agent", L"Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/52.0.2743.116 Safari/537.36");

        return client
            .request(request)
            .then([&](http_response response)
            {
                try 
                {
                    status_code statusCode = response.status_code();
                    if (statusCode == status_codes::OK)
                    {
                        return response.extract_vector().get();
                    }
                    else 
                    {
                        LogRequestError(url, L"Invalid status code: " + to_wstring(statusCode));
                    }
                }
                catch (const http_exception& e)
                {
                    LogRequestException(url, e);
                }

                return vector<unsigned char>();
            })
            .get();
    }
    catch (const std::exception& e)
    {
        LogRequestException(url, e);
        return vector<unsigned char>();
    }
}

wstring RequestProvider::EscapeText(wstring text)
{
    wstring encodedString = uri::encode_data_string(text);
    return encodedString;
}

void RequestProvider::LogRequestException(wstring url, exception exception)
{
    LogRequestError(url, L"Exception: " + StringUtilities::GetUtf16String(exception.what()));
}

void RequestProvider::LogRequestError(wstring url, wstring message)
{
    logger->Log(L"Error requesting URL '" + url + L"'. " + message);
}

RequestProvider::~RequestProvider()
{
}
#include "PrecompiledHeaders\stdafx.h"
#include "Helpers\RequestHelper.h"

using namespace web;
using namespace web::http;
using namespace web::http::client;

wstring RequestHelper::GetStringResponse(wstring url)
{
	vector<unsigned char> response = GetResponse(url);
	
	wstring result = Utilities::GetUtf16String(string(response.begin(), response.end()));

	response.clear();

	return result;
}

vector<unsigned char> RequestHelper::GetResponse(wstring url)
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

wstring RequestHelper::EscapeText(wstring text)
{
	wstring encodedString = uri::encode_data_string(text);
	return encodedString;
}

void RequestHelper::LogRequestException(wstring url, exception exception)
{
	LogRequestError(url, L"Exception: " + Utilities::GetUtf16String(exception.what()));
}

void RequestHelper::LogRequestError(wstring url, wstring message)
{
	Logger::Log(L"Error requesting URL '" + url + L"'. " + message);
}

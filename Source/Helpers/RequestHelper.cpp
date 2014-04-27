#include "PrecompiledHeaders\stdafx.h"
#include "Helpers\RequestHelper.h"

string RequestHelper::GetResponse(string url)
{
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();

	string downloadedResponse;
	
	curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0");

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &downloadedResponse);
		res = curl_easy_perform(curl);

		curl_easy_cleanup(curl);
		curl_slist_free_all(headers);

		if (CURLE_OK == res)
		{
			return downloadedResponse;
		}			
	}
	return NULL;
}

string RequestHelper::EscapeText(string text)
{
	CURL *curl = curl_easy_init();

	char* escapedParameters = curl_easy_escape(curl, text.c_str(), 0);
	string result = string(escapedParameters);
	
	curl_free(escapedParameters);
	curl_easy_cleanup(curl);

	return result;
}

int RequestHelper::Writer(void *data, size_t size, size_t nmemb, string *buffer_in)
{
	size_t realsize = size * nmemb;
	buffer_in->append((char*)data, realsize);
	return realsize;
}

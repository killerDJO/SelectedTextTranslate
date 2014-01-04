#include "Include\stdafx.h"
#include "Include\Translator.h"

TranslateResult Translator::GetTranslationResults()
{
	string selectedText = GetSelectedText();
	string translatorResponse = GetTraslatorJSONResponse(selectedText);
	auto transltorResult = ParseJSONResponse(translatorResponse);
	transltorResult.Word = selectedText;

	return transltorResult;
}

string Translator::GetSelectedText()
{
	string result = "";

	LPGUITHREADINFO lpgui = NULL;

	int key_count = 4;
	INPUT* input = new INPUT[key_count];
	for( int i = 0; i < key_count; i++ )
	{
		input[i].ki.dwFlags = 0;
		input[i].type = INPUT_KEYBOARD;
	}

	input[0].ki.wVk = VK_CONTROL;
	input[0].ki.wScan = MapVirtualKey( VK_CONTROL, MAPVK_VK_TO_VSC );
	input[1].ki.wVk = 0x43; 
	input[1].ki.wScan = MapVirtualKey( 0x43, MAPVK_VK_TO_VSC );
	input[2].ki.dwFlags = KEYEVENTF_KEYUP;
	input[2].ki.wVk = input[0].ki.wVk;
	input[2].ki.wScan = input[0].ki.wScan;
	input[3].ki.dwFlags = KEYEVENTF_KEYUP;
	input[3].ki.wVk = input[1].ki.wVk;
	input[3].ki.wScan = input[1].ki.wScan;

	SendInput( key_count, (LPINPUT)input, sizeof(INPUT) );

	// Waiting for input event
	Sleep(100);

	if( OpenClipboard(NULL) )
	{
		HGLOBAL hglb = GetClipboardData(CF_UNICODETEXT);
		TCHAR* lpstr = (TCHAR*)GlobalLock(hglb);

		if(lpstr)
		{
			CHAR buffer[65535];
			int len = WideCharToMultiByte(20866,0,lpstr,-1,buffer,65535,NULL,NULL);
			result = string(buffer);
		}

		GlobalUnlock(hglb);
		CloseClipboard();
	}

	return result;
}

string Translator::GetTraslatorJSONResponse(string text)
{
	CURL *curl;
    CURLcode res;
    curl = curl_easy_init();

	struct curl_slist *headers=NULL;
	curl_slist_append( headers, "Accept-Charset: utf-8"); 
	curl_slist_append(headers, "Content-Type: application/json; charset=UTF-8");
	string downloadedResponse;
	char* escapedParameters = curl_easy_escape(curl,text.c_str(),0);
	string URL = "http://translate.google.ru/translate_a/t?client=x&hl=en&sl=en&tl=ru&text=" + string(escapedParameters);
	curl_free(escapedParameters);

    if (curl) 
    {
		curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1); 
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA , &downloadedResponse);
        res = curl_easy_perform(curl);		

        if (CURLE_OK == res) 
        { 
            char *ct;         
            res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
            if((CURLE_OK == res) && ct)
                return downloadedResponse;
        }
		curl_easy_cleanup(curl);		
    }
	return "";
}

int Translator::Writer(void *data, size_t size, size_t nmemb, std::string *buffer_in)
{
    size_t realsize = size * nmemb;
    buffer_in->append((char*)data, realsize);
    return realsize;
}   

TranslateResult Translator::ParseJSONResponse(string json)
{
	Json::Value root;  
	Json::Reader reader;
	TranslateResult result;

	bool parsingSuccessful = reader.parse(json, root );
	if (!parsingSuccessful)
	{
		return result;;
	}

	Json::Value sentences = root.get("sentences","[]");
	if(sentences.size() > 0)
	{
		int idx = 0;
		result.TranslatedWord = sentences[idx].get("trans","").asString();
	}

	Json::Value dict = root.get("dict","[]");
	for(size_t i = 0; i < dict.size(); ++i)
	{
		TranslateResultCategory category;
		category.CategoryName = dict[i].get("pos","").asString();
		Json::Value entries = dict[i].get("entry","[]");
		for (size_t j = 0; j < entries.size(); ++j)
		{
			TranslateResultWord word;
			word.Word = entries[j].get("word","").asString();
			word.Score = entries[j].get("score",0.0).asDouble();
			
			category.Words.push_back(word);
		}

		result.Categories.push_back(category);
	}
	return result;
}
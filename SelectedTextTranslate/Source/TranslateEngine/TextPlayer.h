#pragma once
#include "PrecompiledHeaders\stdafx.h"
#include "Helpers\RequestHelper.h"
#include "Helpers\Utilities.h"
#include "TranslateEngine\Translator.h"
#include "Loggers\Logger.h"

#define AUDIO_FILE_NAME "STT_audio"

class TextPlayer
{
private:
	static string GetAudioFilePath(string extension);
	static string SaveToFile(vector<unsigned char> content);
	static DWORD WINAPI Play(LPVOID arg);
	static wchar_t buffer[1000];
public:
	static void PlayText(const wchar_t* text);
};
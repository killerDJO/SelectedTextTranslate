#pragma once
#include "stdafx.h"

#define AUDIO_FILE_NAME "STT_audio"

class TextPlayer
{
private:
	static string GetAudioFilePath(string extension);
	static string SaveToFile(string text);
	static DWORD WINAPI Play(LPVOID arg);
	static wchar_t buffer[1000];
public:
	static void PlayText(wchar_t* text);
};
#pragma once
#include "stdafx.h"

#define AUDIO_FILE_NAME "STT_audio"

class TextPlayer
{
private:
	static string GetAudioFilePath(string extension);
	static string SaveToFile(string text);
public:
	static void PlayText(string text);
};
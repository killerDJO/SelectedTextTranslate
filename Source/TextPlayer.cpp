#include "stdafx.h"
#include "TextPlayer.h"
#include "RequestHelper.h"

string TextPlayer::GetAudioFilePath(string extension)
{
	char buffer[MAX_PATH];
	DWORD pathLength = GetTempPathA(MAX_PATH, buffer);
	buffer[pathLength] = 0;
	return string(buffer) + string(AUDIO_FILE_NAME) + extension;
}

string TextPlayer::SaveToFile(string text)
{
	string path = TextPlayer::GetAudioFilePath(".mp3");
	HANDLE hFile = CreateFileA(path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	DWORD dwWritten;
	WriteFile(hFile, text.c_str(), sizeof(char) * text.length(), &dwWritten, 0);
	CloseHandle(hFile);
	return path;
}

void TextPlayer::PlayText(string text)
{
	string responseQuery = "http://translate.google.com/translate_tts?tl=en&q=" + RequestHelper::EscapeText(text);
	string audio = RequestHelper::GetResponse(responseQuery);
	
	string filePath = SaveToFile(audio);
	
	string openFileCommand = "open " + filePath + " type mpegvideo alias " + string(AUDIO_FILE_NAME);	
	mciSendStringA(openFileCommand.c_str(), NULL, 0, 0);
	
	string playAudioCommand = "play " + string(AUDIO_FILE_NAME) + " wait";
	mciSendStringA(playAudioCommand.c_str(), NULL, 0, 0);

	string closeAudioCommand = "close " + string(AUDIO_FILE_NAME);
	mciSendStringA(closeAudioCommand.c_str(), NULL, 0, 0);
}
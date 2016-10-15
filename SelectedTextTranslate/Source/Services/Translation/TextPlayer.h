#pragma once
#include "Providers\RequestProvider.h"
#include "Services\Translation\Translator.h"
#include "Logging\Logger.h"

#define AUDIO_FILE_NAME "STT_audio"

class TextPlayer
{
private:
    const wchar_t* currentTextToPlay;
    Translator* translator;
    RequestProvider* requestProvider;
    Logger* logger;

    string GetAudioFilePath(string extension) const;
    string SaveToFile(vector<unsigned char> content) const;

    static DWORD WINAPI Play(LPVOID arg);

public:
    TextPlayer(Logger* logger, Translator* translator, RequestProvider* requestProvider);
    ~TextPlayer();

    void PlayText(const wchar_t* text);
};
#pragma once
#include "Providers\RequestProvider.h"
#include "Services\Translation\TranslationService.h"
#include "Infrastructure\Logging\Logger.h"
#include "Infrastructure\ErrorHandling\ErrorHandler.h"

#define AUDIO_FILE_NAME "STT_audio"

class TextPlayer
{
private:
    const wchar_t* currentTextToPlay;

    TranslationService* translationService;
    RequestProvider* requestProvider;
    ErrorHandler* errorHandler;
    Logger* logger;

    string GetAudioFilePath(string extension) const;
    string SaveToFile(vector<unsigned char> content) const;

    static DWORD WINAPI Play(LPVOID arg);

public:
    TextPlayer(Logger* logger, TranslationService* translator, RequestProvider* requestProvider, ErrorHandler* errorHandler);
    ~TextPlayer();

    void PlayText(const wchar_t* text);
};
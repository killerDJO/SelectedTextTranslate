#pragma once
#include "Infrastructure\Logging\Logger.h"
#include "Infrastructure\ErrorHandling\ErrorHandler.h"
#include "DataAccess\RequestProvider.h"
#include "BusinessLogic\Translation\TranslationService.h"

#define AUDIO_FILE_NAME "STT_audio"

class TextPlayer
{
private:
    wstring currentTextToPlay;

    TranslationService* translationService;
    RequestProvider* requestProvider;
    ErrorHandler* errorHandler;
    Logger* logger;

    string GetAudioFilePath(string extension) const;
    string SaveToFile(vector<unsigned char> content) const;

    static DWORD WINAPI Play(LPVOID arg);

public:
    TextPlayer(CompositionRoot* root, ErrorHandler* errorHandler);

    void PlayText(wstring text);
};
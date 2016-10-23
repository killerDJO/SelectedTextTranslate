#include "Services\Translation\TextPlayer.h"
#include "Infrastructure\ErrorHandling\ExceptionHelper.h"
#include "Infrastructure\ErrorHandling\Exceptions\SelectedTextTranslateFatalException.h"

TextPlayer::TextPlayer(Logger* logger, TranslationService* translationService, RequestProvider* requestProvider, ErrorHandler* errorHandler)
{
    this->translationService = translationService;
    this->requestProvider = requestProvider;
    this->logger = logger;
    this->errorHandler = errorHandler;
}

void TextPlayer::PlayText(wstring text)
{
    currentTextToPlay = text;
    DWORD threadId;
    HANDLE hThread = CreateThread(nullptr, 0, TextPlayer::Play, this, 0, &threadId);
    AssertWinApiResult(hThread);
    AssertWinApiResult(CloseHandle(hThread));
}

DWORD WINAPI TextPlayer::Play(LPVOID arg)
{
    TextPlayer* textPlayer = (TextPlayer*)arg;
    try
    {
        ExceptionHelper::SetupStructuredExceptionsTranslation();

        wstring text = wstring(textPlayer->currentTextToPlay);

        textPlayer->logger->Log(L"Start playing sentence '" + text + L"'.");

        wstring responseQuery = L"https://translate.google.com/translate_tts?tl=en&client=t&q=" + textPlayer->requestProvider->EscapeText(text)
            + L"&tk=" + textPlayer->translationService->GetHash(text);

        vector<unsigned char> audio = textPlayer->requestProvider->GetResponse(responseQuery);

        string filePath = textPlayer->SaveToFile(audio);

        audio.clear();

        string openFileCommand = "open " + filePath + " type mpegvideo alias " + string(AUDIO_FILE_NAME);
        mciSendStringA(openFileCommand.c_str(), nullptr, 0, nullptr);

        string playAudioCommand = "play " + string(AUDIO_FILE_NAME) + " wait";
        mciSendStringA(playAudioCommand.c_str(), nullptr, 0, nullptr);

        string closeAudioCommand = "close " + string(AUDIO_FILE_NAME);
        mciSendStringA(closeAudioCommand.c_str(), nullptr, 0, nullptr);

        textPlayer->logger->Log(L"End playing sentence.");

        return 0;
    }
    catch (const SelectedTextTranslateBaseException& error)
    {
        ExceptionHelper::HandleNonFatalException(textPlayer->logger, textPlayer->errorHandler, L"Error playing sentence.", error);
    }
    catch(...)
    {
        ExceptionHelper::HandleNonFatalException(textPlayer->logger, textPlayer->errorHandler, L"Error playing sentence.");
    }

    return -1;
}

string TextPlayer::SaveToFile(vector<unsigned char> content) const
{
    string path = TextPlayer::GetAudioFilePath(".mp3");
    HANDLE hFile = CreateFileA(path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    AssertWinApiHandleResult(hFile);

    DWORD dwWritten;
    AssertWinApiResult(WriteFile(hFile, &content[0], sizeof(unsigned char) * content.size(), &dwWritten, nullptr));
    AssertWinApiResult(CloseHandle(hFile));
    return path;
}

string TextPlayer::GetAudioFilePath(string extension) const
{
    char buffer[MAX_PATH];
    DWORD pathLength = GetTempPathA(MAX_PATH, buffer);
    AssertWinApiResult(pathLength);

    buffer[pathLength] = 0;
    return string(buffer) + string(AUDIO_FILE_NAME) + extension;
}

TextPlayer::~TextPlayer()
{
}
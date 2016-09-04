#include "TranslateEngine\TextPlayer.h"

TextPlayer::TextPlayer(Logger* logger, Translator* translator, RequestProvider* requestProvider)
{
    this->translator = translator;
    this->requestProvider = requestProvider;
    this->logger = logger;
}

void TextPlayer::PlayText(const wchar_t* text)
{
    this->currentTextToPlay = text;
    DWORD threadId;
    HANDLE hThread = CreateThread(NULL, 0, TextPlayer::Play, this, 0, &threadId);
    CloseHandle(hThread);
}

DWORD WINAPI TextPlayer::Play(LPVOID arg)
{
    TextPlayer* textPlayer = (TextPlayer*)arg;
    wstring text = wstring(textPlayer->currentTextToPlay);

    textPlayer->logger->Log(L"Start playing sentence '" + text + L"'.");

    wstring responseQuery = L"http://translate.google.com/translate_tts?tl=en&client=t&q=" + textPlayer->requestProvider->EscapeText(text)
        + L"&tk=" + textPlayer->translator->GetHash(text);

    vector<unsigned char> audio = textPlayer->requestProvider->GetResponse(responseQuery);

    string filePath = textPlayer->SaveToFile(audio);

    audio.clear();

    string openFileCommand = "open " + filePath + " type mpegvideo alias " + string(AUDIO_FILE_NAME);
    mciSendStringA(openFileCommand.c_str(), NULL, 0, 0);

    string playAudioCommand = "play " + string(AUDIO_FILE_NAME) + " wait";
    mciSendStringA(playAudioCommand.c_str(), NULL, 0, 0);

    string closeAudioCommand = "close " + string(AUDIO_FILE_NAME);
    mciSendStringA(closeAudioCommand.c_str(), NULL, 0, 0);

    textPlayer->logger->Log(L"End playing sentence.");

    return 0;
}

string TextPlayer::SaveToFile(vector<unsigned char> content)
{
    string path = TextPlayer::GetAudioFilePath(".mp3");
    HANDLE hFile = CreateFileA(path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    DWORD dwWritten;
    WriteFile(hFile, &content[0], sizeof(unsigned char) * content.size(), &dwWritten, 0);
    CloseHandle(hFile);
    return path;
}

string TextPlayer::GetAudioFilePath(string extension)
{
    char buffer[MAX_PATH];
    DWORD pathLength = GetTempPathA(MAX_PATH, buffer);
    buffer[pathLength] = 0;
    return string(buffer) + string(AUDIO_FILE_NAME) + extension;
}

TextPlayer::~TextPlayer()
{
}
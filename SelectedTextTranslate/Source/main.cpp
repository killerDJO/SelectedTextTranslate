#include "TranslateEngine\Translator.h"
#include "TranslateEngine\TranslatePageParser.h"
#include "Helpers\RequestProvider.h"
#include "Windows\MainWindow.h"
#include "Loggers\Logger.h"

void AttachConsole()
{
    AllocConsole();
    AttachConsole(GetCurrentProcessId());
    FILE* file;
    freopen_s(&file, "CON", "w", stdout);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR args, int iCmdShow )
{ 
    // AttachConsole();

    HANDLE mutex = CreateMutex(NULL,FALSE,_T("Selected text translate"));
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        FatalAppExit(0, TEXT("SelectedTextTranslate already started!"));
    }

    Logger* logger = new Logger();
    DictionaryLogger* dictionaryLogger = new DictionaryLogger(logger);
    TextExtractor* textExtractor = new TextExtractor();
    RequestProvider* requestProvider = new RequestProvider(logger);
    TranslatePageParser* translatePageParser = new TranslatePageParser(logger, requestProvider);
    Translator* translator = new Translator(logger, requestProvider, translatePageParser);
    TextPlayer* textPlayer = new TextPlayer(logger, translator, requestProvider);

    AppModel * appModel = new AppModel(translator, textPlayer, textExtractor, dictionaryLogger);
    MainWindow* mainWindow = new MainWindow(hInstance, appModel);
    appModel->SetMainWindow(mainWindow);

    mainWindow->Initialize();
    
    logger->Log(L"Application start.");

    MSG msg;
    while (GetMessage (&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    logger->Log(L"Application shutdown.");

    ReleaseMutex(mutex);
    CloseHandle(mutex);

    delete logger;
    delete dictionaryLogger;
    delete textExtractor;
    delete requestProvider;
    delete translatePageParser;
    delete translator;
    delete appModel;
    delete textPlayer;
    delete mainWindow;

    return msg.wParam;
}
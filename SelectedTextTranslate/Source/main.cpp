#include "TranslateEngine\Translator.h"
#include "TranslateEngine\TranslatePageParser.h"
#include "Helpers\RequestProvider.h"
#include "Windows\MainWindow.h"
#include "Windows\Framework\Dto\WindowDescriptor.h"
#include "Loggers\Logger.h"

void AttachConsole()
{
    AllocConsole();
    AttachConsole(GetCurrentProcessId());
    FILE* file;
    freopen_s(&file, "CON", "w", stdout);
}

WindowDescriptor GetMainWindowDescriptor(RenderingContext* renderingContext)
{
    int padding = 5;

    RECT workarea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &workarea, 0);

    int width = renderingContext->Scale(300);
    int height = renderingContext->Scale(400);

    int x = workarea.right - width - padding;
    int y = workarea.bottom - height - padding;

    WindowDescriptor descriptor = WindowDescriptor::CreateWindowDescriptor(x, y, width, height, OverflowModes::Scroll, OverflowModes::Scroll);

    return descriptor;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR args, int iCmdShow)
{
    //AttachConsole();

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

    RenderingContext* renderingContext = new RenderingContext();
    ScrollProvider* scrollProvider = new ScrollProvider(renderingContext);

    AppModel * appModel = new AppModel(translator, textPlayer, textExtractor, dictionaryLogger);
    MainWindow* mainWindow = new MainWindow(hInstance, appModel, renderingContext, scrollProvider, GetMainWindowDescriptor(renderingContext));
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
    delete renderingContext;
    delete scrollProvider;

    return msg.wParam;
}